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

#include <bier/parser/ast/ast_node.h>
#include <bier/core/types_registry.h>

namespace bier {

namespace ast {

class ModuleAst {
public:
    explicit ModuleAst(std::unique_ptr<DefaultTypesRegistry> types
                       = std::make_unique<DefaultTypesRegistry>())
        : types_(std::move(types)){
    }

    TypeRegistryInterface* Types() const {
        return types_.get();
    }

private:
    std::unique_ptr<DefaultTypesRegistry> types_;
};

}   // ast

}   // bier
