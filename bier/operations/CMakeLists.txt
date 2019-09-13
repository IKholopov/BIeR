# Build default operations library

add_library(bier_ops
    alloc_layout.cpp
    branch.cpp
    call.cpp
    cast.cpp
    const_op.cpp
    gep.cpp
    return.cpp)
target_include_directories(bier_ops PUBLIC ${BIER_INC})
target_cxx(bier_ops)
