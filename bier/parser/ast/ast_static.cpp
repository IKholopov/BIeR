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
#include "ast_static.h"
#include <bier/core/const_value.h>
#include <bier/parser/exceptions.h>

namespace bier {

namespace ast {

ValuePtr StaticIntEntry::Make(const Module&) const {
    auto int_type = dynamic_cast<const IntTypeBase*>(type_);
    check(int_type != nullptr, LanguageException("Not an integer type for integer const",
                                                 FilePosition()));
    return std::make_unique<IntegerConst>(value_, int_type);
}

void StaticIntEntry::VerifyIsOfType(const Type* type, const TypeRegistryInterface*) const {
    check(type == type_, LanguageException("Type in static data does not match", FilePosition()));
}

StaticFuncEntry::StaticFuncEntry(const FunctionType* type,
                                 FuncSignature&& signature, const FilePos& pos)
    : ASTBase<StaticFuncEntry, StaticEntry>(pos),
      signature_(std::move(signature)) {
    checkSame(type);
}

ValuePtr StaticFuncEntry::Make(const Module& module) const {
    const bier::FunctionSignature* func_sig = module.GetFunctionSignature(signature_.name);
    const std::string& name = signature_.name;
    check(signature_.return_type == func_sig->FuncType()->ReturnType(),
          LanguageException("Function signature does not match return type for " + name,
                            FilePosition()));
    check(signature_.args.size() == func_sig->Arguments().Size(),
          LanguageException("Function signature does not match arguments count for " + name,
                            FilePosition()));
    const auto args = func_sig->Arguments();
    for (size_t i = 0; i < args.Size(); ++i) {
        const auto val = *(args.begin() + i);
        check(signature_.args[i].type == val->GetType(),
              LanguageException("Function signature does not match argument " + std::to_string(i)
                                +  " count for " + name,
                                FilePosition()));
    }
    return std::make_unique<bier::FunctionPointer>(func_sig);
}

void StaticFuncEntry::VerifyIsOfType(const Type* type, const TypeRegistryInterface* types) const {
    if (types->DefaultTypes()->GetPtr() == type) {
        return;
    }
    auto func_type = dynamic_cast<const FunctionType*>(type);
    check(func_type != nullptr,
          LanguageException("Type in static data does not match", FilePosition()));
    checkSame(func_type);
}

void StaticFuncEntry::checkSame(const FunctionType* type) const {
    const std::string& name = signature_.name;
    check(type->ReturnType() == signature_.return_type,
          LanguageException("Function signature does not match return type for " + name,
                            FilePosition()));
    check(signature_.args.size() == type->Arguments().size(),
          LanguageException("Function signature does not match arguments count for " + name,
                            FilePosition()));
    const auto& args = type->Arguments();
    for (size_t i = 0; i < args.size(); ++i) {
        const auto val = *(args.begin() + i);
        check(signature_.args[i].type == val,
              LanguageException("Function signature does not match argument " + std::to_string(i)
                                +  " count for " + name,
                                FilePosition()));
    }
}

}   // ast

}   // bier
