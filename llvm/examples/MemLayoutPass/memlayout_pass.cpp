//===-- HelloWorld.cpp - Example Transformations --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include "memlayout_pass.h"

using namespace llvm;

PreservedAnalyses MemLayoutPass::run(Function &F, FunctionAnalysisManager &AM) {

    errs() << "=======================================================" << F.getName() << "\n";
    errs() << "cs642_memlayout_experiment: " << F.getName() << ": Entering pass\n";

    if (F.hasFnAttribute(Attribute::ReadNone)) {
        errs() << "cs642_memlayout_experiment: " << F.getName() << ": Early exit: readnone\n";
        return PreservedAnalyses::all();
    }

    errs() << "cs642_memlayout_experiment: " << F.getName() << ": Exiting pass\n";
    errs() << "=======================================================" << F.getName() << "\n";

    // Not sure which analyses are still good after this, so we are conservative and invalidate all of them
    return PreservedAnalyses::none();
}



// Boilerplate for dynamically loading the plugin
llvm::PassPluginLibraryInfo getMemLayoutPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "MemLayoutPass", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
        PB.registerVectorizerStartEPCallback(
            [](llvm::FunctionPassManager &PM, OptimizationLevel Level) {
                PM.addPass(MemLayoutPass());
            });
        PB.registerPipelineParsingCallback(
            [](StringRef Name, llvm::FunctionPassManager &PM,
                ArrayRef<llvm::PassBuilder::PipelineElement>) {
                if (Name == "goodbye") {
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
