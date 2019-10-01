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

