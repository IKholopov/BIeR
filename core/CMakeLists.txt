# Build core library

file(GLOB bier_core_SRC function.cpp
    type.cpp)

message(${BIER_INC})

add_library(bier_core ${bier_core_SRC})
target_include_directories(bier_core PUBLIC ${BIER_INC})
