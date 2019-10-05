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
#include <bier/core/static_data.h>
#include <bier/operations/opcodes.h>
#include <boost/functional/hash.hpp>

namespace bier {

DagView::DagView(const BasicBlock* block, BasicBlock::ConstOperationIterator operation_it)
    : content_(operation_it),
      context_(DagContext::Make(block)) {
}

DagView::DagView(const BasicBlock* block, const Value* value)
    : content_(value),
      context_(DagContext::Make(block)){
}

std::optional<DagView> DagView::SeqLink() const {
    if (content_.index() == kValueIndex
            || asOp() == block()->GetOperations().begin()) {
        return std::nullopt;
    }

    auto op = asOp();
    --op;
    return DagView(op, context_);
}

std::vector<DagView> DagView::Links() const {
    std::vector<DagView> links;
    if (content_.index() == kValueIndex) {
        return links;
    }

    const Operation* op = asOp()->get();
    std::vector<const Value*> arguments = op->GetArguments();
    for (const auto arg : arguments) {
        auto arg_op = arg->GetOp();
        if (arg_op.has_value() && context_->Has(arg_op.value())
                && arg_op.value()->OpCode() != OpCodes::Op::ALLOC_OP) {
            links.push_back(DagView(context_->Get(arg_op.value()),
                                    context_));
            continue;
        }

        links.push_back(DagView(arg, context_));
    }

    return links;
}

DagNodeType DagView::GetType() const {
    if (content_.index() == 0) {
        return DagNodeType::OPERATION;
    }
    const Value* value = AsVal();
    if (value->GetOp().has_value()) {
        return value->GetOp().value()->OpCode() == OpCodes::Op::ALLOC_OP ? DagNodeType::ALLOCA
                                                                         : DagNodeType::EXTERNAL_OPERATION;
    }
    if (dynamic_cast<const ArgumentValue*>(value) != nullptr) {
        return DagNodeType::ARG;
    }
    if (dynamic_cast<const Variable*>(value) != nullptr) {
        return DagNodeType::MUTABLE;
    }
    if (dynamic_cast<const ConstValue*>(value) != nullptr) {
        return DagNodeType::CONST;
    }
    if (dynamic_cast<const FunctionSignature*>(value) != nullptr) {
        return DagNodeType::SIGNATURE;
    }
    if (dynamic_cast<const StaticData*>(value) != nullptr) {
        return DagNodeType::STATIC_DATA;
    }
    assert(false);
}

const Operation* DagView::AsOp() const {
    return asOp()->get();
}

DagView DagView::Root(const BasicBlock* block) {
    return {block, --block->GetOperations().end()};
}

bool DagView::operator==(const DagView& other) const {
    if (context_.get() != other.context_.get()
            || content_.index() != other.content_.index()) {
        return false;
    }
    if (content_.index() == kOpIndex) {
        return asOp()->get() == other.asOp()->get();
    }
    return AsVal() == other.AsVal();
}

DagView::DagView(BasicBlock::ConstOperationIterator operation_it,
                 DagContextPtr context)
    : content_(operation_it),
      context_(context) {
}

DagView::DagView(const Value* value,
                 DagContextPtr context)
    : content_(value),
      context_(context) {
}

const BasicBlock::ConstOperationIterator DagView::asOp() const {
    return std::get<BasicBlock::ConstOperationIterator>(content_);
}

const Value* DagView::AsVal() const {
    return std::get<const Value*>(content_);
}

HashType DagView::Hash::operator()(const DagView& dag) const {
    HashType hash = 0;
    boost::hash_combine(hash, dag.context_.get());
    if (dag.content_.index() == kOpIndex) {
        boost::hash_combine(hash, dag.asOp()->get());
    } else {
        boost::hash_combine(hash, dag.AsVal());
    }
    return hash;
}

}   // _bier
