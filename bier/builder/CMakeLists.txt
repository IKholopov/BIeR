# Build builder classes

add_library(bier_builder
    module_builder.cpp
    verifier.cpp)
target_include_directories(bier_builder PUBLIC ${BIER_INC})
target_link_libraries(bier_builder PUBLIC bier_core)
target_cxx(bier_builder)
