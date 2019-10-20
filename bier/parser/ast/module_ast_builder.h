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
#include <bier/parser/ast/module_ast.h>

namespace bier {

namespace ast {

class ModuleAstBuilder {
public:
    explicit ModuleAstBuilder(ModuleAst* ast) : ast_(ast) {
        initOpCodes();
    }

    int OpCode(const std::string& name, const FilePos& pos) const;

    const Type* GetIntType(int bits_count,
                           const FilePos& pos) const;
    const Type* GetPtrType() const;
    const Type* GetPtrToType(const Type* type_of) const;
    const FunctionType* GetFuncType(std::optional<const Type*> return_type,
                                    std::vector<const Type*>&& arguments = {}) const;
    const TypeRegistryInterface* Types() const {
        return ast_->Types();
    }

private:
    ModuleAst* ast_;
    StdHashMap<std::string, int> nameToCode;

    void initOpCodes();
};

}   // ast

}   // bier
