# Build serialization classes

add_library(bier_serialization
    text_serializer.cpp
    op_literals.cpp)
target_include_directories(bier_serialization PUBLIC ${BIER_INC})
target_cxx(bier_serialization)
