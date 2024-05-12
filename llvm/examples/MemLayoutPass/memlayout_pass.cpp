//===-- HelloWorld.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Transforms/Utils/Cloning.h"


using namespace llvm;

static cl::opt<std::string> TargetFunction(
        "memlayout-target", 
        cl::init(""), 
        cl::desc("Function to target for memory layout rejiggery (defaults to all!)"));


struct MemLayoutPass : public PassInfoMixin<MemLayoutPass> {

    void wrapFunction(Function& Outer, Function& Inner) {
        Inner.setName(Outer.getName() + "_inner");
        Outer.deleteBody(); // Sets linkage to external
                            // TODO: It may be better to just clone the FunctionType, etc
                            // There's just so many things attached to Function that I don't understand yet
        Outer.setLinkage(Inner.getLinkage());

        // Forces the compiler to keep the inner function around, which may come in handy 
        // later... or may interfere with vectorization... we'll see
        Inner.setLinkage(GlobalValue::LinkageTypes::ExternalLinkage);
        
        BasicBlock *BB = BasicBlock::Create(Outer.getContext(), "entry", &Outer);
        IRBuilder<> Builder(BB);
    
        // For now we just forward the args from Outer straight to Inner
        std::vector<llvm::Value*> InnerArgs;
        for (llvm::Argument& Arg : Outer.args()) {
            InnerArgs.push_back(&Arg);
            // Maybe I want to clone these?
        }
        llvm::CallInst *Call = Builder.CreateCall(&Inner, InnerArgs);

        // Return result argument
        Builder.CreateRet(Call);
    }


    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MM) {

        errs() << "=======================================================\n";
        errs() << "memlayout: " << M.getName() << ": Entering pass\n";

        std::vector<Function*> Targets;
        for (Function& F : M) {
            // Skip if the function is already annotated as "readnone"
            if (F.hasFnAttribute(Attribute::ReadNone)) {
                errs() << "memlayout: " << F.getName() << ": Early exit: readnone\n";
                continue;
            }

            // Skip if we specified a different target function via command line
            if (!TargetFunction.getValue().empty() && F.getName() != TargetFunction.getValue()) {
                errs() << "memlayout: " << F.getName() 
                        << ": Early exit: Doesn't match target function '" << TargetFunction.getValue() << "'\n";
                continue;
            }

            Targets.push_back(&F);
        }

        for (Function* F : Targets) {
            // Actually do the work
            errs() << "memlayout: " << F->getName() << ": Wrapping function\n";
            //Function inner(F.getFunctionType(), F.getLinkage(), F.getAddrSpace());

            ValueToValueMapTy ValueMap;
            ClonedCodeInfo Info;
            Function* Inner = CloneFunction(F, ValueMap, &Info);
            wrapFunction(*F, *Inner);
        }

        errs() << "memlayout: " << M.getName() << ": Exiting pass\n";
        errs() << "=======================================================\n";

        // Not sure which analyses are still good after this, so we are conservative and invalidate all of them
        return PreservedAnalyses::none();
    }
};



// Boilerplate for dynamically loading the plugin
llvm::PassPluginLibraryInfo getMemLayoutPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "MemLayoutPass", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
        /*
        PB.registerVectorizerStartEPCallback(
            [](llvm::FunctionPassManager &PM, OptimizationLevel Level) {
                PM.addPass(MemLayoutPass());
            });
        */
        // TODO: We probably DO want to run this pass right before the vectorizer, but let's
        // hold off until we understand more about how the new pass manager works
        PB.registerPipelineParsingCallback(
            [](StringRef Name, llvm::ModulePassManager &PM,
                ArrayRef<llvm::PassBuilder::PipelineElement>) {
                if (Name == "memlayout") {
                    PM.addPass(MemLayoutPass());
                    return true;
                }
                return false;
            });
        }
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getMemLayoutPassPluginInfo();
}
