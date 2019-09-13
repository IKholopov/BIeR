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

#include <bier/core/operation.h>
#include <bier/operations/opcodes.h>

namespace bier {

class Branch {
public:
    virtual ~Branch() = default;
    virtual std::vector<const BasicBlock*> DestinationBlocks() const = 0;
};

class BranchOperation
        : public BaseOperation<OpCodes::Op::BRANCH_OP>,
          public Branch {
public:
    BranchOperation(const Function* context, const BasicBlock* target);

    // Интерфейс Operation
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>&) override {}
    std::optional<const Value*> GetReturnValue() const override;
    void SubstituteReturnValue(const Value*) override {}

    // Интерфейс FunctionContextMember
    const Function* GetContextFunction() const override {
        return context_;
    }

    // Интерфейс Branch
    std::vector<const BasicBlock*> DestinationBlocks() const override;

private:
    const Function* context_ = nullptr;
    const BasicBlock* target_ = nullptr;
};

class ConditionalBranchOperation
        : public BaseOperation<OpCodes::Op::COND_BRANCH_OP>,
          public Branch {
public:
    ConditionalBranchOperation(const Function* context, const Value* condition,
                               const BasicBlock* target_true, const BasicBlock* target_false);

    // Интерфейс operation
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Value*> GetReturnValue() const override;
    void SubstituteReturnValue(const Value*) override {}

    // Интерфейс FunctionContextMember
    const Function* GetContextFunction() const override {
        return context_;
    }

    // Интерфейс Branch
    std::vector<const BasicBlock*> DestinationBlocks() const override;

private:
    const Function* context_ = nullptr;
    const BasicBlock* target_true_ = nullptr;
    const BasicBlock* target_false_ = nullptr;
    const Value* condition_ = nullptr;
};

}   // _bier
