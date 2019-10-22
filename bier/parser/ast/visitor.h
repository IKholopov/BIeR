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
#include <bier/parser/ast/node_types.h>

namespace bier {

namespace ast {

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void Visit(const StaticDataEntry*) {};
    virtual void Visit(const StaticFuncEntry*) {};
    virtual void Visit(const StaticIntEntry*) {};
    virtual void Visit(const RegularOp*) {};
    virtual void Visit(const GepOp*) {};
    virtual void Visit(const BranchOp*) {};
    virtual void Visit(const Block*) {};
    virtual void Visit(const ExternFunction*) {};
    virtual void Visit(const DefinedFunction*) {};
    virtual void Visit(const ASTLayout*) {};
};

}   // AST

}   // bier
