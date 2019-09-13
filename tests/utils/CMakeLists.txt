add_executable(utils_tests
    utils_tests.cpp
    opcodes_literal_test.cpp
)
target_include_directories(utils_tests PUBLIC ${CATCH_PATH} ${BIER_INC})
target_link_libraries(utils_tests bier_serialization)
target_cxx(utils_tests)
