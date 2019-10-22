# Build parser_ast
add_library(bier_parser_ast
        ast_block.cpp
        ast_func.cpp
        ast_layout.cpp
        ast_static.cpp
        module_ast.cpp
        module_ast_builder.cpp
)
target_include_directories(bier_parser_ast PUBLIC ${BIER_INC})
target_cxx(bier_parser_ast)
target_link_libraries(bier_parser_ast PUBLIC bier_core bier_serialization)
