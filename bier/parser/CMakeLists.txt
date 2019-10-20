# Build parser
add_subdirectory(ast)

flex_target(bier_parser_flex lexems.ll ${CMAKE_CURRENT_BINARY_DIR}/lexems.gen.cpp)
bison_target(bier_parser_bison parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.gen.cpp VERBOSE )

add_flex_bison_dependency(bier_parser_flex bier_parser_bison)

add_library(bier_parser
        bier_scanner.cpp
	${FLEX_bier_parser_flex_OUTPUTS}
        ${BISON_bier_parser_bison_OUTPUTS}
)
target_cxx(bier_parser)
add_library(bier::bier_parser ALIAS bier_parser)
target_include_directories(bier_parser PUBLIC ${BIER_INC} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(bier_parser PUBLIC bier_parser_ast)

add_executable(bier_lexprinter
        bier_lexprinter.cpp)
target_cxx(bier_lexprinter)
add_executable(bier::bier_lexprinter ALIAS bier_lexprinter)
target_link_libraries(bier_lexprinter PRIVATE bier_parser)


add_executable(bier_parserprinter
        bier_parserprinter.cpp)
target_cxx(bier_parserprinter)
add_executable(bier::bier_parserprinter ALIAS bier_parserprinter)
target_link_libraries(bier_parserprinter PRIVATE bier_parser)

if(BUILD_TESTS)
    file(GLOB LexerSamples "${TEST_PT_SRC_DIR}/*.pt")

    foreach(sample ${LexerSamples})
        add_test("Lexer_${sample}" bier_lexprinter "${sample}")
    endforeach()
endif()
