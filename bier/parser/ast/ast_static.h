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
#include <bier/core/function.h>
#include <bier/parser/ast/ast_node.h>

namespace bier {

namespace ast {

class StaticIntEntry : public ASTBase<StaticIntEntry, StaticEntry> {
public:
    StaticIntEntry(const Type* type, uint64_t value, const FilePos& pos)
        : ASTBase<StaticIntEntry, StaticEntry>(pos),
          value_(value),
          type_(type) {
    }

    ValuePtr Make(const Module& module) const override;
    void VerifyIsOfType(const Type* type, const TypeRegistryInterface*) const override;

private:
    uint64_t value_ = 0;;
    const Type* type_ = nullptr;
};

class StaticFuncEntry : public ASTBase<StaticFuncEntry, StaticEntry> {
public:
    StaticFuncEntry(const bier::FunctionType* type, FuncSignature&& signature,
                    const FilePos& pos);

    ValuePtr Make(const Module& module) const override;
    void VerifyIsOfType(const Type* type, const TypeRegistryInterface* types) const override;

private:
    FuncSignature signature_;
    void checkSame(const bier::FunctionType* type) const;
};

class StaticDataEntry : public ASTBase<StaticDataEntry>{
public:
    StaticDataEntry(const std::string& name, std::vector<StaticEntryPtr>&& data, const FilePos& pos)
        : ASTBase(pos),
          data_(std::move(data)),
          name_(name) {
    }

    const std::vector<StaticEntryPtr>& Data() const {
        return data_;
    }
    const std::string& Name() const {
        return name_;
    }

private:
    std::vector<StaticEntryPtr> data_;
    std::string name_;
};

using StaticDataEntryPtr = std::unique_ptr<StaticDataEntry>;

}   // ast

}   // bier
