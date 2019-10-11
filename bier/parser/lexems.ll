%{
    #include <bier/parser/bier_scanner.h>
    using namespace bier;

    #undef YY_DECL
    #define YY_DECL int bier::BierScanner::yylex(yy::parser::semantic_type* const yylval, yy::parser::location_type* location)
%}

%option c++
%option yyclass="BierScanner"
%option noyywrap nounput noinput

SPACE   [ \t]+
CHAR    [A-Za-z_]
DIGITP  [1-9]
ZERO    0
DIGIT   {ZERO}|{DIGITP}

INT_VAL ({DIGITP}({DIGIT})*|{ZERO})
ID      ({CHAR}|{DIGIT})*

%%

%{
    value_ = yylval;
%}

{SPACE}                 {   Space(location);    }
"\n"                    {   NewLine();  }
"global"                {   return Process(Token::GLOBAL, location);   }
"func"                  {   return Process(Token::FUNC, location);  }
"<"                     {   return Process(Token::L_ANG_BR, location);  }
">"                     {   return Process(Token::R_ANG_BR, location);  }
"{"                     {   return Process(Token::L_FIG_BR, location);  }
"}"                     {   return Process(Token::R_FIG_BR, location);  }
"["                     {   return Process(Token::L_SQ_BR, location);  }
"]"                     {   return Process(Token::R_SQ_BR, location);  }
"("                     {   return Process(Token::L_BR, location);  }
")"                     {   return Process(Token::R_BR, location);  }
"ptr"                   {   return Process(Token::PTR, location);  }
"i"{INT_VAL}            {   return Process(Token::I_TYPE, location);  }
"*"                     {   return Process(Token::STAR, location);  }
"="                     {   return Process(Token::EQ, location);  }
"%"                     {   return Process(Token::PERCENT, location);  }
"$"                     {   return Process(Token::DOLLAR, location);  }
"\""                    {   return Process(Token::QUOTE, location);  }
"x"                     {   return Process(Token::X, location);  }
","                     {   return Process(Token::COMMA, location);  }
":"                     {   return Process(Token::COLON, location);  }
"@"                     {   return Process(Token::AT, location);  }
"idx"                   {   return Process(Token::IDX, location);  }
{ID}                    {   return Process(Token::ID, location);  }
.                       {   Error(); }

%%