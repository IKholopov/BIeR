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
#include "basic_block.h"

namespace bier {

void BasicBlock::Append(bier::OperationPtr&& operation) {
    check(!branch_terminated_, std::runtime_error("trying to add operation to block with branch at the end"));
    operations_.emplace_back(std::move(operation));
}

void BasicBlock::InsertAt(BasicBlock::OperationIterator iterator, OperationPtr&& operation) {
    operations_.insert(iterator, std::move(operation));
}

void BasicBlock::DeleteAt(BasicBlock::OperationIterator iterator) {
    operations_.erase(iterator);
}

const ConstValue* BasicBlock::InsertConst(std::unique_ptr<ConstValue>&& value) {
    assert(value.get() != nullptr);
    const ConstValue* ptr = value.get();
    constants_.emplace_back(std::move(value));
    return ptr;
}

const Function* BasicBlock::GetContextFunction() const {
    return context_;
}

}   // _bier

