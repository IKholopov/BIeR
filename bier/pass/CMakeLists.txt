# Build pass library

add_library(bier_pass
    operation_pass.cpp
    ssa_pass.cpp)
target_include_directories(bier_pass PUBLIC ${BIER_INC})
target_cxx(bier_pass)
