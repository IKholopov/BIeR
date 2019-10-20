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

#include <bier/core/type.h>
#include <string>
#include <variant>
#include <vector>

namespace bier {

namespace ast {

struct VarName {
    std::string name;
    bool is_mutable = false;
};

struct VarEntry {
    const Type* type = nullptr;
    VarName name;
};

struct IntConst {
    const Type* type = nullptr;
    uint64_t value = 0;
};

using OpArg = std::variant<VarEntry, IntConst>;

struct Operation {
    int OpCode = -1;
    std::vector<OpArg> args;
    std::optional<VarEntry> return_var;
};

struct FuncSignature {
    std::string name;
    std::vector<VarEntry> args;
    std::optional<const Type*> return_type;
};

struct LayoutEntry {
    const Type* type = nullptr;
    uint64_t count = 1;
};

}   // ast

}   // bier
