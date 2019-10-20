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
#include "bier_scanner.h"
#include <bier/parser/ast/module_ast_builder.h>
#include <fstream>
#include <string>
#include <unordered_map>

#include <thread>

static const std::unordered_map<bier::Token, std::string> kTokenNames = {
    {bier::Token::GLOBAL, "GLOBAL"},
    {bier::Token::FUNC, "FUNC"},
    {bier::Token::L_ANG_BR, "L_ANG_BR"},
    {bier::Token::R_ANG_BR, "R_ANG_BR"},
    {bier::Token::L_FIG_BR, "L_FIG_BR"},
    {bier::Token::R_FIG_BR, "R_FIG_BR"},
    {bier::Token::L_SQ_BR, "L_SQ_BR"},
    {bier::Token::R_SQ_BR, "R_SQ_BR"},
    {bier::Token::L_BR, "L_BR"},
    {bier::Token::R_BR, "R_BR"},
    {bier::Token::PTR, "PTR"},
    {bier::Token::I_CONST, "I_CONST"},
    {bier::Token::I_TYPE, "I_TYPE"},
    {bier::Token::VOID, "VOID"},
    {bier::Token::STAR, "STAR"},
    {bier::Token::EQ, "EQ"},
    {bier::Token::VAR, "VAR"},
    {bier::Token::MUTABLE_VAR, "MUTABLE_VAR"},
    {bier::Token::QUOTE, "QUOTE"},
    {bier::Token::X, "X"},
    {bier::Token::COMMA, "COMMA"},
    {bier::Token::COLON, "COLON"},
    {bier::Token::GEP, "GEP"},
    {bier::Token::BRANCH, "BRANCH"},
    {bier::Token::COND_BRANCH, "COND_BRANCH"},
    {bier::Token::ALLOC_LAYOUT, "ALLOC_LAYOUT"},
    {bier::Token::ID, "ID"},
    {bier::Token::IDX, "IDX"},
    {bier::Token::AT, "AT"}
};

class PrintTokenCallBack : public bier::TokenCallback {
public:
    explicit PrintTokenCallBack(std::ostream& stream) : stream_(stream) {}

    void Process(bier::Token token, const std::string& content,
                 const bier::Position& pos) override;
    void NewLine() override;

private:
    std::ostream& stream_;
};

static void report_error(const std::string& message) {
    std::cerr << "\x1B[31m" << message << "\x1B[0m" << std::endl;
}

int main(int argc, const char* argv[]) {
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    PrintTokenCallBack callback{std::cerr};

    bier::BierScanner scanner{&callback};
    std::ifstream file_stream;

    if (argc > 1) {
        file_stream.open(argv[1]);
        if (!file_stream.is_open()) {
            std::cerr << "failed to open a file " << argv[1] << std::endl;
            return 1;
        }
        scanner.switch_streams(file_stream, std::cerr);
    }
    bier::ast::ModuleAst ast;
    bier::ast::ModuleAstBuilder astBuilder{&ast};
    yy::parser parser(scanner, astBuilder);
    try {
        if( parser.parse() != 0 ) {
            report_error( "parsing failed" );
            return 1;
        }
    } catch (std::exception& e) {
        report_error( e.what() );
        return 1;
    }

    return 0;
}

void PrintTokenCallBack::Process(bier::Token token, const std::string& content,
                                 const bier::Position& pos) {
    stream_ << kTokenNames.at(token) << "[\'" << content << "\']("
            << pos.begin.line << "," << pos.begin.column << ") ";
}

void PrintTokenCallBack::NewLine() {
    stream_ << std::endl;
}
