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

#include <bier/pass/operation_pass.h>

namespace bier {

class SSAPass : public OperationPass {
protected:
    // OperationPass interface
    void OnFunction(Function* function) override;
    OperationIterator OperationTransformation(BasicBlock* block,
                                              OperationIterator iterator) override;

private:
    StdHashMap<const Value*, const Value*> mutable_values_;
    StdHashSet<const Value*> allocated_;
    Function* function_ = nullptr;

    const Value* MakeLoad(BasicBlock* block, OperationIterator iterator,
                          const Value* to_load) const;
    const Value* MakeStore(BasicBlock* block, OperationIterator iterator,
                           const Value* to_store) const;
};

}  // namespace bier
