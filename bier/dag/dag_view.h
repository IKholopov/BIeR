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
#include <bier/dag/dag_context.h>
#include <variant>

namespace bier {

enum class DagNodeType {
    SIGNATURE,
    CONST,
    STATIC_DATA,
    ALLOCA,
    ARG,
    MUTABLE,
    OPERATION,
    EXTERNAL_OPERATION
};

class DagView {
public:
    struct Hash {
        HashType operator()(const DagView& dag) const;
    };

    DagView(const BasicBlock* block, BasicBlock::ConstOperationIterator operation_it);
    DagView(const BasicBlock* block, const Value* value); 

    std::optional<DagView> SeqLink() const;
    std::vector<DagView> Links() const;

    DagNodeType GetType() const;
    const Operation* AsOp() const;
    const Value* AsVal() const;

    static DagView Root(const BasicBlock* block);

    bool operator==(const DagView& other) const;
    bool operator!=(const DagView& other) const {
        return !(*this == other);
    }

protected:
    DagView(BasicBlock::ConstOperationIterator operation_it,
            DagContextPtr context);
    DagView(const Value* value, DagContextPtr context);

private:
    static const size_t kOpIndex = 0;
    static const size_t kValueIndex = 1;

    std::variant<BasicBlock::ConstOperationIterator, const Value*> content_;
    DagContextPtr context_;

    const BasicBlock* block() const {
        return context_->GetBlock();
    }
    const BasicBlock::ConstOperationIterator asOp() const;
};

}   // _bier
