CLANG_BUILD_DIR = ../build
CLANG = ${CLANG_BUILD_DIR}/bin/clang
OPT = ${CLANG_BUILD_DIR}/bin/opt

.PHONY: clang tests_all tests_cpp tests_ir tests_cpp_memoryssa tests_ir_memoryssa

clang:
	echo "Building clang"
	cmake --build ${CLANG_BUILD_DIR}

tests_all: tests_cpp tests_ir
	echo "Building all tests"

tests_cpp:

	echo "Generating pre-pass IR for C++ tests"
	${CLANG} memlayout_tests.cpp -O3 -S -emit-llvm -o memlayout_tests_pre.ll

	echo "Generating post-pass IR for C++ tests"
	${OPT} -passes='print<memoryssa>',helloworld,'print<memoryssa>' -S -o memlayout_tests_post.ll memlayout_tests_pre.ll

	echo "Compiling C++ tests"
	${CLANG} memlayout_tests_post.ll -o memlayout_tests.out


tests_ir:

	echo "Generating post-pass IR for IR tests"
	${OPT} -passes='print<memoryssa>',helloworld,'print<memoryssa>' -S -o memlayout_tests_ir_post.ll memlayout_tests_ir.ll


