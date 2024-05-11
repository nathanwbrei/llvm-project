
# CS642 Memlayout Pass Experiments
By Nathan Brei

## Basic idea
This is an experiment in reorganizing memory via an LLVM pass, in order to make an impure function pure. This may be helpful for vectorizing legacy code and/or injecting surrogate models. For more information, see PROJECT_REPORT.md. 

## Files
- `./memlayout_tests.cpp`: Simple test cases in C++. The idea is to lower these to llvm IR that we can explicitly inspect pre-pass and post-pass, or compile all the way through so we can run it and test the assertions. 

- `./memlayout_tests_ir_pre.ll`: These are even simpler test cases written directly in llvm IR, that reduce the visual noise compared to the `memlayout_tests.cpp`.

- `./Makefile`: Contains recipes for running the llvm optimizer that generates inspectable output. Note that the pass itself is built through the standard LLVM CMake build.

The following files get generated via our Makefile:

- `./memlayout_tests.out`: The executable version of our test cases
- `./memlayout_tests_pre.ll`: The LLVM IR lowering of `memlayout_tests.cpp`, before running our pass
- `./memlayout_tests_post.ll`: The LLVM IR lowering of `memlayout_tests.cpp`, after running our pass
- `./memlayout_tests_ir_post.ll`: The LLVM IR lowering of `memlayout_tests_ir_pre.ll`, after running our pass

Note that we are checking the generated files into our repository so that people can observe our results without having to build all of llvm and clang. It is also convenient for immediately highlighting diffs in the output IR without having to invest in using a real testing framework such as LLVM's `lit`.



