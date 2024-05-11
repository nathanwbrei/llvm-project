//===-- HelloWorld.h - Example Transformations ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CS642_MEMLAYOUT_PASS_H
#define CS642_MEMLAYOUT_PASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class MemLayoutPass : public PassInfoMixin<MemLayoutPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // CS642_MEMLAYOUT_PASS_H
