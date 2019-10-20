%require "3.2"
%language "c++"

%define api.value.type variant

%locations

%code requires{
    #include <bier/core/function.h>
    #include <bier/parser/ast/ast_node.h>
    #include <bier/parser/ast/ast_static.h>
    namespace bier {
        class BierScanner;
        namespace ast {
            class ModuleAst;
            class ModuleAstBuilder;
        }
    }

    using namespace bier::ast;

}

%parse-param { bier::BierScanner& scanner }
%parse-param { bier::ast::ModuleAstBuilder& ast }

%code {
    #include <bier/common.h>
    #include <bier/core/layout.h>
    #include <bier/core/type.h>
    #include <bier/parser/ast/ast_static.h>
    #include <bier/parser/ast/module_ast_builder.h>
    #include <bier/parser/bier_scanner.h>

#undef yylex
#define yylex scanner.yylex

    static bier::FilePos to_pos(const bier::Position& pos) {
        return {pos.begin.line, pos.begin.column};
    }

}

%token GLOBAL
%token FUNC
%token L_FIG_BR
%token R_FIG_BR
%token L_ANG_BR
%token R_ANG_BR
%token L_SQ_BR
%token R_SQ_BR
%token L_BR
%token R_BR
%token PTR
%token <uint64_t> I_CONST
%token <uint64_t> I_TYPE
%token VOID
%token STAR
%token <std::string> VAR
%token <std::string> MUTABLE_VAR
%token EQ
%token QUOTE
%token X
%token COMMA
%token COLON
%token GEP
%token BRANCH
%token COND_BRANCH
%token ALLOC_LAYOUT
%token <std::string> ID
%token IDX
%token AT

%token END 0

%type module
%type top_entry
%type top_entry_list

%type <bier::ast::StaticDataEntryPtr> static_data
%type <bier::ast::StaticEntryPtr> static_data_entry
%type <std::vector<bier::ast::StaticEntryPtr>> static_data_entry_list
%type <bier::ast::StaticEntryPtr> static_value

%type func_declaration
%type func_definition
%type <const bier::FunctionType*> func_sig_type
%type <FuncSignature> func_signature
%type func_body
%type func_body_entry

%type <std::string> label
%type <Operation> operation
%type <Operation> op
%type <bier::ast::OpArg> op_arg
%type <std::vector<bier::ast::OpArg>> op_args

%type layout
%type <std::string> layout_name_decl
%type layout_contents
%type layout_contents_list
%type <bier::ast::LayoutEntry> layout_entry

%type <bier::ast::VarEntry> arg
%type <std::vector<bier::ast::VarEntry>> arg_list

%type <const bier::Type*> basic_type
%type <std::vector<const bier::Type*>> basic_type_list
%type <const bier::Type*> ext_type
%type <std::optional<const bier::Type*>> return_type

%type <bier::ast::VarName> var_name

%%

module
    : END
    | top_entry_list END

top_entry_list
    : top_entry
    | top_entry_list top_entry

top_entry
    : static_data
    | layout
    | func_declaration
    | func_definition

static_data
    : GLOBAL ID L_ANG_BR static_data_entry_list R_ANG_BR    { $$ = std::make_unique<StaticDataEntry>($2, std::move($4), to_pos(@1)); }

static_data_entry_list
    : static_data_entry                             { $$.emplace_back(std::move($1)); }
    | static_data_entry_list static_data_entry      { $1.emplace_back(std::move($2)); $$ = std::move($1); }

static_data_entry
    : basic_type static_value           { $2->VerifyIsOfType($1, ast.Types()); $$ = std::move($2); }

static_value
    : func_sig_type func_signature      { $$ = std::make_unique<StaticFuncEntry>($1, std::move($2), to_pos(@1)); }
    | I_TYPE I_CONST                    { $$ = std::make_unique<StaticIntEntry>(ast.GetIntType($1, to_pos(@1)), $2, to_pos(@1)); }

arg
    : basic_type var_name     { $$ = VarEntry{$1, $2}; }

arg_list
    : arg                     { $$ = std::vector{$1}; }
    | arg_list COMMA arg      { $1.push_back($3); $$ = std::move($1); }

func_signature
    : FUNC ID L_BR arg_list R_BR return_type    { $$ = FuncSignature{std::move($2), std::move($4), $6}; }
    | FUNC ID L_BR R_BR return_type             { $$ = FuncSignature{std::move($2), {}, $5}; }

basic_type
    : I_TYPE                { $$ = ast.GetIntType($1, to_pos(@1)); }
    | PTR                   { $$ = ast.GetPtrType(); }
    | basic_type STAR       { $$ = ast.GetPtrToType($1); }

return_type
    : basic_type   { $$ = $1; }
    | VOID         { $$ = std::nullopt; }

func_sig_type
    : L_BR R_BR return_type                   { $$ = ast.GetFuncType($3); }
    | L_BR basic_type_list R_BR return_type   { $$ = ast.GetFuncType($4, std::move($2)); }

ext_type
    : func_sig_type   { $$ = $1; }
    | basic_type      { $$ = $1; }

basic_type_list
    : basic_type                          { $$ = std::vector{$1}; }
    | basic_type_list COMMA basic_type    { $1.push_back($3); $$ = std::move($1); }

func_definition
    : func_signature L_FIG_BR func_body R_FIG_BR

func_declaration
    : func_signature

func_body
    : func_body_entry
    | func_body func_body_entry

func_body_entry
    : label
    | operation

label
    : ID COLON         { $$ = $1; }

operation
    : var_name basic_type EQ op    { $4.return_var = VarEntry{$2, $1}; $$ = std::move($4); }
    | op                           { $$ = std::move($1); }

var_name
    : VAR              { $$ = VarName{$1, false}; }
    | MUTABLE_VAR      { $$ = VarName{$1, true}; }

op
    : ID               { $$ = Operation{ast.OpCode($1, to_pos(@1)), {}, std::nullopt}; }
    | ID op_args       { $$ = Operation{ast.OpCode($1, to_pos(@1)), std::move($2), std::nullopt}; }
    | gep_op
    | cond_op
    | branch_op
    | alloc_layout_op

op_args
    : op_arg                { $$ = { $1 }; }
    | op_args COMMA op_arg  { $1.push_back($3); $$ = std::move($1); }

op_arg
    : ext_type var_name     { $$ = VarEntry{$1, $2}; }
    | I_TYPE I_CONST        { $$ = IntConst{ ast.GetIntType($1, to_pos(@1)), $2}; }

gep_op
    : GEP AT ID IDX I_CONST op_arg
    | GEP anonymous_layout IDX I_CONST op_arg

cond_op
    : COND_BRANCH op_arg ID COMMA ID

branch_op
     : BRANCH ID

alloc_layout_op
     : ALLOC_LAYOUT AT ID I_CONST

layout
     : layout_name_decl layout_contents

anonymous_layout
     : QUOTE QUOTE layout_contents

layout_name_decl
     : QUOTE ID QUOTE   { $$ = $2; }

layout_contents
     : QUOTE L_SQ_BR R_SQ_BR QUOTE
     | QUOTE L_SQ_BR layout_contents_list R_SQ_BR QUOTE

layout_contents_list
     : layout_entry
     | layout_contents_list COMMA layout_entry

layout_entry
     : L_SQ_BR basic_type X I_CONST R_SQ_BR     { $$ = bier::ast::LayoutEntry{$2, $4}; }

%%

void yy::parser::error (const location_type& l, const std::string& m) {
    std::cerr << "\x1B[31m" << m << " at " << l << "\x1B[0m" << std::endl;
}
