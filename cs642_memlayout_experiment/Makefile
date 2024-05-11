CLANG_BUILD_DIR = ../build
CLANG = ${CLANG_BUILD_DIR}/bin/clang
OPT = ${CLANG_BUILD_DIR}/bin/opt

.PHONY: clang tests_all tests_cpp tests_ir

clang:
	echo "Building clang"
	cmake --build ${CLANG_BUILD_DIR}

tests_all: tests_cpp tests_ir
	echo "Building all tests"

tests_cpp:

	echo "Building C++ tests"
	${CLANG} memlayout_tests.cpp -o memlayout_tests.out

	echo "Generating pre-pass IR for C++ tests"
	${CLANG} memlayout_tests.cpp -O0 -S -emit-llvm -o memlayout_tests_pre.ll

	echo "Generating post-pass IR for C++ tests"
	${OPT} -passes=helloworld -S -o memlayout_tests_post.ll memlayout_tests_pre.ll

tests_ir:

	echo "Generating post-pass IR for IR tests"
	${OPT} -passes=helloworld -S -o memlayout_tests_ir_post.ll memlayout_tests_ir.ll

