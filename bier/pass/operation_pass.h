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
#include <bier/pass/transform_pass.h>

namespace bier {

class OperationPass : public TransformPass {
public:
    using OperationIterator = BasicBlock::OperationIterator;

    // ModulePass interface
    void Apply(ModulePtr&& module) override;

    // TransformPass interface
    ModulePtr GetTransformed() override;

protected:
    virtual void OnFunction(Function* function);
    virtual OperationIterator OperationTransformation(BasicBlock* block,
                                                      OperationIterator iterator);
    DefaultTypesRegistry* Types();

private:
    ModulePtr current_module_;
};

}  // namespace bier
