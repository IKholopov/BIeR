/*
   Copyright 2019 Igor Kholopov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#if !defined (yyFlexLexerOnce)
#include <FlexLexer.h>
#endif // _!defined (yyFlexLexerOnce)

#include <parser.gen.hpp>

namespace bier {

using Token = yy::parser::token::yytokentype;
using Position = yy::parser::location_type;
using Semantic = yy::parser::semantic_type;

class TokenCallback {
public:
    virtual ~TokenCallback() = default;
    virtual void Process(Token token, const std::string& content,
                         const Position& pos) = 0;
    virtual void NewLine() = 0;
};

class BierScanner : public yyFlexLexer {
public:
    ~BierScanner();
    explicit BierScanner(TokenCallback* callback = nullptr) : callback_(callback) {
    }

    int yylex(Semantic* const yylval,
              Position* location);

private:
    TokenCallback* callback_ = nullptr;
    Semantic* value_ = nullptr;
    unsigned int col = 0;
    unsigned int line = 0;

    int Process(Token token, Position* location);
    void FillValue(Token token);
    void UpdatePosition(Position* location);
    void NewLine();
    void Space(Position* location);
    [[noreturn]]
    void Error();
};

}   // bier
