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
#include "dag_context.h"
#include <bier/core/function.h>
#include <bier/operations/opcodes.h>

namespace bier {

void DagContext::Build(const BasicBlock* block) {
    block_ = block;
    op_to_iterator_.clear();
    const Function* funciton = block->GetContextFunction();
    for (const auto& block : funciton->GetBlocks()) {
        for (const auto& op : block.GetOperations()) {
            auto return_value = op->GetReturnValue();
            if (return_value.has_value()) {
                value_to_op_.insert({return_value.value(), op.get()});
            }
        }
    }

    auto range = block->GetOperations();
    for (auto it = range.begin(); it != range.end(); ++it) {
        const Operation* op = it->get();
        op_to_iterator_.insert({op, it});
    }
    for (const auto& op : range) {
        auto args = op->GetArguments();
        for (const Value* arg : args) {
            auto dependency = GetOp(arg);
            AddDependencies(op.get(), dependency);
        }
    }
}

BasicBlock::ConstOperationIterator DagContext::Get(const Operation* op) const {
    assert(ContainerHas(op_to_iterator_, op));
    return op_to_iterator_.at(op);
}

const DagContext::DependentOps& DagContext::GetDependent(const Operation* op) const {
    if (!ContainerHas(op_dependent_, op)) {
        return empty_;
    }
    return op_dependent_.at(op);
}

bool DagContext::Has(const Operation* op) const {
    return ContainerHas(op_to_iterator_, op);
}

DagContextPtr DagContext::Make(const BasicBlock* block) {
    auto context = std::make_shared<DagContext>();
    context->Build(block);
    return context;
}

void DagContext::AddDependencies(const Operation* op,
                                 std::optional<const Operation*> dependency) {
    if (!dependency.has_value()) {
        return;
    }
    const Operation* dependency_op = dependency.value();
    if (ContainerHas(op_to_iterator_, dependency_op)) {
        auto& dependents = op_dependent_[op];
        dependents.emplace_back(op_to_iterator_.at(dependency_op));
    }
}

std::optional<const Operation*> DagContext::GetOp(const Value* value) const {
    if (!ContainerHas(value_to_op_, value)) {
        return std::nullopt;
    }
    return value_to_op_.at(value);
}

}   // bier
