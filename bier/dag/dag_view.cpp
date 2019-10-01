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
#include "dag_view.h"

namespace bier {

DagView::DagView(const BasicBlock* block, BasicBlock::OperationIterator operation_it)
    : content_(operation_it),
      block_(block) {
}

DagView::DagView(const BasicBlock* block, const Value* value)
    : content_(value),
      block_(block) {
}

std::optional<DagView> DagView::SeqLink() const {
    if (content_.index() == kValueIndex
            || std::get<BasicBlock::OperationIterator>(content_) == block_->GetOperations().begin()) {
        return std::nullopt;
    }

    auto op = as_op();
    --op;
    return DagView(block_, op);
}

std::vector<DagView> DagView::Links() const {
    std::vector<DagView> links;
    if (content_.index() == kValueIndex) {
        return links;
    }

    const Operation* op = as_op()->get();
    std::vector<const Value*> arguments = op->GetArguments();
    for (const auto arg : arguments) {
        auto arg_op = arg->GetOp();
        if (arg_op.has_value()) {

        }
    }
}

const BasicBlock::OperationIterator DagView::as_op() const {
    return std::get<BasicBlock::OperationIterator>(content_);
}

}   // _bier
