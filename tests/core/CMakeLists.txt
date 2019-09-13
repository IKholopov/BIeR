add_executable(core_tests
    core_tests.cpp
    functions_declaration_test.cpp
    layout_test.cpp
    type_registry_test.cpp)
target_include_directories(core_tests PUBLIC ${CATCH_PATH} ${BIER_INC})
target_link_libraries(core_tests bier_core)
target_cxx(core_tests)
