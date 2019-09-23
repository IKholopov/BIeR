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
#include <bier/common.h>
#include <bier/core/const_value.h>
#include <bier/core/function_context.h>
#include <bier/core/operation.h>
#include <bier/utils/iterator_range.h>
#include <bier/utils/intrusive_list.h>
#include <list>
#include <vector>

namespace bier {

class BasicBlock : public IntrusiveListNode<BasicBlock>, public FunctionContextMemeber {
public:
    template <typename T>
    using OperationContainer = std::list<T>;
    using OperationIterator = OperationContainer<OperationPtr>::iterator;

    explicit BasicBlock(const Function* function, const std::string& label = "")
        : label_(label), context_(function) {
    }

    void Append(OperationPtr&& operation);
    void InsertAt(OperationIterator iterator, OperationPtr&& operation);
    void DeleteAt(OperationIterator iterator);
    const ConstValue* InsertConst(std::unique_ptr<ConstValue>&& value);
    void TerminateBlock() {
        branch_terminated_ = true;
    }

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;
    auto GetOperations() const {
        return IteratorRange(operations_);
    }
    auto GetOperations() {
        return MutableIteratorRange<decltype(operations_)>(operations_);
    }

    const std::string& GetLabel() const {
        return label_;
    }

private:
    std::vector<std::unique_ptr<ConstValue>> constants_;
    OperationContainer<OperationPtr> operations_;
    std::string label_;
    const Function* context_ = nullptr;
    bool branch_terminated_ = false;
};

}  // namespace bier
