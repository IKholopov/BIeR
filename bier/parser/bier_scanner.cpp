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
#include "exceptions.h"

namespace bier {

BierScanner::~BierScanner() {
}

int BierScanner::Process(Token token, Position* location) {
    UpdatePosition(location);
    if (callback_ != nullptr) {
        callback_->Process(token, YYText(), *location);
    }
    FillValue(token);
    value_ = nullptr;
    return token;
}

void BierScanner::FillValue(Token token) {
    switch(token) {
        case Token::I_TYPE: {
            std::string value = YYText();
            assert(value.size() > 1);
            value_->build<uint64_t>(std::stoull(value.c_str() + 1));
        }
            return;
        case Token::I_CONST: {
            value_->build<uint64_t>(std::stoull(YYText()));
        }
        return;
        case Token::ID:
            value_->build<std::string>(YYText());
            return;
        case Token::VAR:{
            std::string value = YYText();
            assert(value.size() >= 1);
            value_->build<std::string>(value.c_str() + 1);
        }
        return;
        case Token::MUTABLE_VAR:{
            std::string value = YYText();
            assert(value.size() >= 1);
            value_->build<std::string>(value.c_str() + 1);
        }
        return;
        default:
            return;
    }
}

void BierScanner::UpdatePosition(Position* location) {
    location->begin.line = line;
    location->begin.column = col;
    col += static_cast<unsigned>(YYLeng());
    location->end.line = line;
    location->end.column = line;
}

void BierScanner::NewLine() {
    ++line;
    col = 0;
    if (callback_ != nullptr) {
        callback_->NewLine();
    }
}

void BierScanner::Space(Position* location) {
    UpdatePosition(location);
}

void BierScanner::Error() {
    throw LexicalException(std::string("Unknown lexem \"") + YYText() + "\"", FilePos{line, col});
}

}   // bier
