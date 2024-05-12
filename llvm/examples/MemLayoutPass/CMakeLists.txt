

add_llvm_pass_plugin(MemLayoutPass memlayout_pass.cpp
    DEPENDS intrinsics_gen BUILDTREE_ONLY)


