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

#include <bier/parser/ast/visitor.h>
#include <bier/parser/bier_scanner.h>
#include <memory>

namespace bier {

namespace AST {

template<typename T>
using ASTPtr = std::unique_ptr<T>;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual Position FilePosition() const = 0;
    virtual void Accept(Visitor* visitor) const = 0;
};

template<typename T, typename TBase=ASTNode>
class ASTBase : public TBase {
public:
    explicit ASTBase(Position pos) : position_(pos) {
    }

    void Accept(Visitor* visitor) const override {
        visitor->Visit(static_cast<const T*>(this));
    }

    Position FilePosition() const override {
        return position_;
    }
private:
    Position position_;
};

}

}
