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
#include "module_ast_builder.h"
#include <bier/operations/opcodes.h>
#include <bier/parser/exceptions.h>
#include <bier/serialization/op_literals.h>

namespace bier {

namespace ast {

int ModuleAstBuilder::OpCode(const std::string& name,
                             const FilePos& pos) const {
    check(ContainerHas(nameToCode, name), LanguageException("Unknown op name " + name, pos));
    return nameToCode.at(name);
}

const Type* ModuleAstBuilder::GetIntType(int bits_count,
                                         const FilePos& pos) const {
    switch (bits_count) {
    case 1:
        return ast_->Types()->DefaultTypes()->GetInt1();
    case 8:
        return ast_->Types()->DefaultTypes()->GetInt8();
    case 16:
        return ast_->Types()->DefaultTypes()->GetInt16();
    case 32:
        return ast_->Types()->DefaultTypes()->GetInt32();
    case 64:
        return ast_->Types()->DefaultTypes()->GetInt64();

    default:
        throw LanguageException("Unknown language type: i" + std::to_string(bits_count), pos);
    }
}

const Type* ModuleAstBuilder::GetPtrType() const {
    return ast_->Types()->DefaultTypes()->GetPtr();
}

const Type* ModuleAstBuilder::GetPtrToType(const Type* type_of) const {
    return ast_->Types()->DefaultTypes()->GetPtrTo(type_of);
}

const FunctionType* ModuleAstBuilder::GetFuncType(std::optional<const Type*> return_type,
                                          std::vector<const Type*>&& arguments) const {
    return ast_->Types()->DefaultTypes()->MakeFunctionType(return_type, arguments);
}

void ModuleAstBuilder::initOpCodes() {
    for (int i = 0; i < OpCodes::OPS_COUNT; ++i) {
        const std::string literal = Literal::OpCodeValue(i);
        assert(!ContainerHas(nameToCode, literal));
        nameToCode.insert({std::move(literal), i});
    }
}

}   // ast

}   // bier
