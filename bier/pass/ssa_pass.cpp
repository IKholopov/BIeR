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
#include "ssa_pass.h"
#include <bier/operations/branch.h>

namespace bier {

void SSAPass::OnFunction(Function* function) {
    function_ = function;

    std::vector<const Value*> values;
    for (const auto& [name, variable] : function->GetVariables()) {
        if (variable->IsMutable()) {
            values.push_back(variable.get());
        }
        variable->MakeImmutable();
    }

    mutable_values_.clear();
    allocated_.clear();
    BasicBlock* start_block = function->CreateBlockAtStart();
    for (const Value* val : values) {
        const Value* one = start_block->InsertConst(std::make_unique<IntegerConst>(
            1, static_cast<const IntTypeBase*>(Types()->GetInt64())));
        const Variable* variable = function->AllocateVariable(
            Variable::Metadata(val->GetName() + "_ptr", Types()->GetPtrTo(val->GetType())));
        auto unOp = std::make_unique<UnaryOperation>(start_block->GetContextFunction(),
                                                     UnaryOperation::UnOp::ALLOC, one, variable);
        start_block->Append(std::move(unOp));
        mutable_values_.insert({val, variable});
        allocated_.insert(variable);
    }
    start_block->Append(
        std::make_unique<BranchOperation>(start_block->GetContextFunction(), start_block->Next()));
}

OperationPass::OperationIterator SSAPass::OperationTransformation(BasicBlock* block,
                                                                  OperationIterator iterator) {
    Operation* op = iterator->get();
    if (op->GetReturnValue().has_value() &&
        ContainerHas(allocated_, op->GetReturnValue().value())) {
        // Skip allocations
        return ++iterator;
    }

    std::vector<const Value*> args;
    OperationIterator next_it = iterator;
    int n_loads = 0;
    for (const Value* val : op->GetArguments()) {
        if (ContainerHas(mutable_values_, val)) {
            args.push_back(MakeLoad(block, next_it, val));
            ++next_it;
            ++n_loads;
        } else {
            args.push_back(val);
        }
    }
    if (n_loads > 0) {
        op->SubstituteArguments(args);
    }
    ++next_it;
    if (op->GetReturnValue().has_value() &&
        ContainerHas(mutable_values_, op->GetReturnValue().value())) {
        op->SubstituteReturnValue(MakeStore(block, next_it, op->GetReturnValue().value()));
        ++next_it;
    }

    return next_it;
}

const Value* SSAPass::MakeLoad(BasicBlock* block, OperationIterator iterator,
                               const Value* to_load) const {
    const Value* loaded = function_->AllocateVariable(
        Variable::Metadata(to_load->GetName() + "_val", to_load->GetType()));
    block->InsertAt(iterator,
                    std::make_unique<UnaryOperation>(function_, UnaryOperation::UnOp::LOAD,
                                                     mutable_values_.at(to_load), loaded));
    return loaded;
}

const Value* SSAPass::MakeStore(BasicBlock* block, OperationPass::OperationIterator iterator,
                                const Value* to_store) const {
    const Value* store_in = function_->AllocateVariable(
        Variable::Metadata(to_store->GetName() + "_val", to_store->GetType()));
    block->InsertAt(iterator, std::make_unique<BinaryOperation>(
                                  function_, BinaryOperation::BinOp::STORE, store_in,
                                  mutable_values_.at(to_store), nullptr));
    return store_in;
}

}  // namespace bier
