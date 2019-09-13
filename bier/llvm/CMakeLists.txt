# Build llvm support library

add_library(bier_llvm
    llvm_pass.cpp)
target_include_directories(bier_llvm PUBLIC ${BIER_INC})
target_cxx(bier_llvm)
