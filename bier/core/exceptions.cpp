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

IRException::IRException(const std::string& base_message, const IRContext& context) {
    messages_.base_message = base_message;
    SetContext(context);
}

IRException::IRException(const std::string& base_message,
                         std::optional<const Function*> function,
                         std::optional<const BasicBlock*> block) {
    messages_.base_message = base_message;
    SetContext(IRContext{function, block, std::nullopt});
}

const char* IRException::what() const noexcept {
    message_ = BuildMessage();
    return message_.c_str();
}

void IRException::SetFunctionMessage(const Function* function) {
    messages_.function_message = "at function " + function->GetName();
}

void IRException::SetBlockMessage(const BasicBlock* block) {
    messages_.block_message = "at block " + block->GetLabel();
}

void IRException::SetPosition(const SourcePos& pos) {
    messages_.pos_message = "at " + std::to_string(pos.line) + ", " + std::to_string(pos.col);
}

std::string IRException::BuildMessage() const {
    std::ostringstream input(Prefix);
    input << messages_.base_message << " ";
    if (!messages_.function_message.empty()) {
        input << messages_.function_message << " ";
    }

    if (!messages_.block_message.empty()) {
        input << messages_.block_message << " ";
    }

    if (!messages_.pos_message.empty()) {
        input << messages_.pos_message << " ";
    }
    return input.str();
}

void IRException::SetContext(const IRContext& context) {
    if (context.function.has_value()) {
        SetFunctionMessage(context.function.value());
    }
    if (context.basic_block.has_value()) {
        SetBlockMessage(context.basic_block.value());
    }
    if (context.source.has_value()) {
        SetPosition(context.source.value());
    }
}

}   // bier
