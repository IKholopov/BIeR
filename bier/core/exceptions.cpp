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
#include "exceptions.h"
#include <bier/core/function.h>
#include <sstream>

static const char* const Prefix = "BIeR IR Exception: ";

namespace bier {

IRException::IRException(const std::string& base_message,
                         std::optional<const Function*> function,
                         std::optional<const BasicBlock*> block)
    : base_message_(base_message) {
    data_.function = function;
    data_.basic_block = block;
}

const char* IRException::what() const noexcept {
    message_ = BuildMessage();
    return message_.c_str();
}

std::string IRException::BuildMessage() const {
    std::ostringstream input(Prefix);
    input << base_message_ << " ";
    if (data_.function.has_value()) {
        input << "at func " << data_.function.value()->GetName();
    }
    return input.str();
}

}   // bier
