add_subdirectory(core)
add_subdirectory(operations)
add_subdirectory(builder)
add_subdirectory(serialization)
add_subdirectory(pass)
add_subdirectory(dag)

find_package(LLVM)

if(${LLVM_FOUND})
    add_subdirectory(llvm)
else()
    message("LLVM libraries not found, LLVM support is disabled")
endif()

find_package(FLEX 2.6)
find_package(BISON 3.2)
set(PARSER_ENABLED ${FLEX_FOUND} AND ${BISON_FOUND})

if(${PARSER_ENABLED})
    add_subdirectory(parser)
else()
   message("Supported versions of FLEX(2.6+)/BISON(3.2+) are missing parser is disabled")
endif()

