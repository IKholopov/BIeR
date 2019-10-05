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

namespace bier {

void DagContext::Build(const BasicBlock* block) {
    block_ = block;
    op_to_iterator_.clear();
    auto range = block->GetOperations();
    for (auto it = range.begin(); it != range.end(); ++it) {
        const Operation* op = it->get();
        op_to_iterator_.insert({op, it});
    }
    for (const auto& op : range) {
        auto args = op->GetArguments();
        for (const Value* arg : args) {
            auto dependent = arg->GetOp();
            if (dependent.has_value()
                    && ContainerHas(op_to_iterator_, dependent.value())) {
                auto& dependents = op_dependent_[op.get()];
                dependents.emplace_back(op_to_iterator_.at(dependent.value()));
            }
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

}   // bier
