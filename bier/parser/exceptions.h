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
#include <stdexcept>

namespace bier {

struct FilePos {
    unsigned int line = 0;
    unsigned int col = 0;
};

class LexicalException : public std::runtime_error {
public:
    LexicalException(const std::string& message, const FilePos& pos)
        : std::runtime_error(message + " at " + std::to_string(pos.line) + "," + std::to_string(pos.col)) {
    }
};

class LanguageException : public std::runtime_error {
public:
    LanguageException(const std::string& message, const FilePos& pos)
        : std::runtime_error(message + " at " + std::to_string(pos.line) + "," + std::to_string(pos.col)) {
    }
};

}   // _bier
