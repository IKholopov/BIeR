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
#include <bier/common.h>
#include <bier/core/function.h>
#include <bier/core/basic_block.h>
#include <stdexcept>

namespace bier {

struct SourcePos {
    int col = 0;
    int line = 0;
};

struct IRContext {
    std::optional<const Function*> function;
    std::optional<const BasicBlock*> basic_block;
    std::optional<SourcePos> source;
};

class IRException : public std::exception {
public:
    IRException(const std::string& base_message, const IRContext& context)
        : data_(context),
          base_message_(base_message) {
    }
    explicit IRException(const std::string& base_message,
                         std::optional<const Function*> function = std::nullopt,
                         std::optional<const BasicBlock*> block = std::nullopt);

    virtual const char* what() const noexcept override;
    IRContext& Context() {
        return data_;
    }

private:
    IRContext data_;
    std::string base_message_;
    mutable std::string message_;

    std::string BuildMessage() const;
};

}   // bier
