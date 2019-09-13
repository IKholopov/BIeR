# Build core library

add_library(bier_core
    basic_block.cpp
    basic_types.cpp
    const_value.cpp
    function.cpp
    label.cpp
    layout.cpp
    module.cpp
    operation.cpp
    types_registry.cpp
    variable.cpp
    variable_name.cpp)
target_include_directories(bier_core PUBLIC ${BIER_INC})
target_cxx(bier_core)
