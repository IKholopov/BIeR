#Build core library

add_library(bier_core STATIC
        basic_block.cpp
        basic_types.cpp
        const_value.cpp
        exceptions.cpp
        function.cpp
        label.cpp
        layout.cpp
        module.cpp
        operation.cpp
        static_data.cpp
        types_registry.cpp
        variable.cpp
        variable_name.cpp)
add_library(bier::bier_core ALIAS bier_core)
target_include_directories(bier_core PUBLIC ${BIER_INC})
target_link_libraries(bier_core PRIVATE Boost::boost)
set_target_properties(bier_core
    PROPERTIES INTERFACE_LINK_LIBRARIES "")
target_cxx(bier_core)
