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
#include "operation_pass.h"

namespace bier {

void OperationPass::Apply(ModulePtr&& module) {
    current_module_ = std::move(module);
    for (auto& [name, func] : current_module_->GetDefinedFunctions()) {
        OnFunction(func.get());
        for (auto& block : func->GetBlocks()) {
            auto ops = block.GetOperations();
            for (auto it = ops.begin(); it != ops.end();) {
                it = OperationTransformation(&block, it);
            }
        }
        func->Normalize();
    }
}

ModulePtr OperationPass::GetTransformed() {
    return std::move(current_module_);
}

void OperationPass::OnFunction(Function* /* function */) {
    return;
}

OperationPass::OperationIterator OperationPass::OperationTransformation(
        BasicBlock* /* block */, OperationIterator iterator) {
    return ++iterator;
}

DefaultTypesRegistry* OperationPass::Types() {
    return current_module_->Types();
}

}   // _bier
