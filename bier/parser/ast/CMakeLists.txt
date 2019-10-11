# Build parser
flex_target(bier_parser_flex lexems.ll ${CMAKE_CURRENT_BINARY_DIR}/lexems.gen.cpp)
bison_target(bier_parser_bison parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.gen.cpp VERBOSE )

add_flex_bison_dependency(bier_parser_flex bier_parser_bison)

add_library(bier_parser
        bier_scanner.cpp
	${FLEX_bier_parser_flex_OUTPUTS}
        ${BISON_bier_parser_bison_OUTPUTS}
)
target_include_directories(bier_parser PUBLIC ${BIER_INC} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(bier_lexprinter
        bier_lexprinter.cpp)
target_link_libraries(bier_lexprinter PRIVATE bier_parser)
