CLANG_BUILD_DIR = ../build
CLANG_INSTALL_DIR = /work/epsci/nbrei/llvm-project/install
CLANG = ${CLANG_BUILD_DIR}/bin/clang
OPT = ${CLANG_BUILD_DIR}/bin/opt

.PHONY: all pass clang_build_from_scratch clang_rebuild tests_all tests_cpp tests_ir tests_cpp_memoryssa tests_ir_memoryssa


all: pass tests_cpp tests_ir
	# Finished

clang_build_from_scratch:
	echo "Building clang"
	mkdir -p ${CLANG_BUILD_DIR}
	cmake -S llvm -B ${CLANG_BUILD_DIR} -G "Unix Makefiles" \
		-DLLVM_ENABLE_PROJECTS="clang" \
		-DCMAKE_INSTALL_PREFIX=${CLANG_INSTALL_DIR} \
		-DCMAKE_BUILD_TYPE=Debug
	cmake --build ../build -j4

clang_rebuild:
	cmake --build ${CLANG_BUILD_DIR} -j 4

pass:
	# Building MemLayoutPass
	cd ${CLANG_BUILD_DIR}/examples/MemLayoutPass; make

tests_cpp:

	# Generating pre-pass IR for C++ tests
	${CLANG} memlayout_tests.cpp -O3 -S -emit-llvm -o memlayout_tests_pre.ll

	# Generating post-pass IR for C++ tests
	${OPT} -load-pass-plugin ${CLANG_BUILD_DIR}/lib/MemLayoutPass.so \
		   -passes='function(print<memoryssa>)',memlayout \
		   -memlayout-target _Z1fi \
		   -S -o memlayout_tests_post.ll memlayout_tests_pre.ll

	# Compiling C++ tests
	${CLANG} memlayout_tests_post.ll -o memlayout_tests.out


tests_ir:

	# Generating post-pass IR for IR tests
	${OPT} -load-pass-plugin ${CLANG_BUILD_DIR}/lib/MemLayoutPass.so \
		-passes='function(print<memoryssa>)',memlayout,'function(print<memoryssa>)' -S -o memlayout_tests_ir_post.ll memlayout_tests_ir.ll


