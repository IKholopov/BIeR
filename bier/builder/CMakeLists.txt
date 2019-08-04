# Build builder classes

file(GLOB bier_builder_SRC module_builder.cpp)

add_library(bier_builder ${bier_builder_SRC})
target_include_directories(bier_builder PUBLIC ${BIER_INC})
