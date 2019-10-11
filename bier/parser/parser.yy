%require "3.2"
%language "c++"

%define api.value.type variant

%locations

%code requires{
    namespace bier {
        class BierScanner;
    }
}

%parse-param { bier::BierScanner& scanner }

%code {
    #include <bier/parser/bier_scanner.h>

#undef yylex
#define yylex scanner.yylex

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
%token <int> I_TYPE
%token STAR
%token PERCENT
%token DOLLAR
%token EQ
%token QUOTE
%token X
%token COMMA
%token COLON
%token <std::string> ID
%token IDX
%token AT

%token END 0

%type module
%type token_list
%type token

%%

module
    : END
    | token_list END

token_list
    : token
    | token_list token

token
    : GLOBAL
    | FUNC
    | L_ANG_BR
    | R_ANG_BR
    | L_FIG_BR
    | R_FIG_BR
    | L_SQ_BR
    | R_SQ_BR
    | L_BR
    | R_BR
    | PTR
    | I_TYPE
    | STAR
    | EQ
    | PERCENT
    | DOLLAR
    | QUOTE
    | X
    | COMMA
    | COLON
    | ID
    | IDX
    | AT

%%

void yy::parser::error (const location_type& l, const std::string& m) {
    std::cerr << m << " at " << l << std::endl;
}
