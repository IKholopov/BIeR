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

#include <bier/core/module.h>
#include <bier/parser/ast/ast_structs.h>
#include <bier/parser/ast/node_types.h>
#include <bier/parser/ast/visitor.h>
#include <bier/parser/exceptions.h>
#include <memory>

namespace bier {

namespace ast {

template<typename T>
using ASTPtr = std::unique_ptr<T>;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual FilePos FilePosition() const = 0;
    virtual void Accept(Visitor* visitor) const = 0;
};

template<typename T, typename TBase=ASTNode>
class ASTBase : public TBase {
public:
    explicit ASTBase(FilePos pos) : position_(pos) {
    }

    void Accept(Visitor* visitor) const override {
        visitor->Visit(static_cast<const T*>(this));
    }

    FilePos FilePosition() const override {
        return position_;
    }
private:
    FilePos position_;
};

class StaticEntry : public ASTNode {
public:
    virtual ValuePtr Make(const Module& module) const = 0;
    virtual void VerifyIsOfType(const Type* type, const TypeRegistryInterface* types) const = 0;
};

using StaticEntryPtr = std::unique_ptr<StaticEntry>;

}

}
