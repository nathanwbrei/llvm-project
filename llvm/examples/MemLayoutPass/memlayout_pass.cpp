//===-- HelloWorld.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"


using namespace llvm;

static cl::opt<std::string> TargetFunction(
        "memlayout-target", 
        cl::init(""), 
        cl::desc("Function to target for memory layout rejiggery (defaults to all!)"));


struct MemLayoutPass : public PassInfoMixin<MemLayoutPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
        if (F.hasFnAttribute(Attribute::ReadNone)) {
            errs() << "memlayout_experiment: " << F.getName() << ": Early exit: readnone\n";
            return PreservedAnalyses::all();
        }

        if (!TargetFunction.getValue().empty() && F.getName() != TargetFunction.getValue()) {
            errs() << "memlayout_experiment: " << F.getName() 
                    << ": Early exit: Doesn't match target function '" << TargetFunction.getValue() << "'\n";
            return PreservedAnalyses::all();
        }

        errs() << "=======================================================\n";
        errs() << "memlayout_experiment: " << F.getName() << ": Entering pass\n";

        errs() << "memlayout_experiment: " << F.getName() << ": Exiting pass\n";
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
            [](StringRef Name, llvm::FunctionPassManager &PM,
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
