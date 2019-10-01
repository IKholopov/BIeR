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
#include <bier/core/function.h>
#include <bier/core/operation.h>
#include <bier/operations/opcodes.h>

namespace bier {

class ReturnVoidOp : public BaseOperation<OpCodes::RETVOID_OP> {
public:
    explicit ReturnVoidOp(const Function* context_func) : context_(context_func) {
    }

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>&) override {
    }
    std::optional<const Variable*> GetReturnValue() const override;
    void SubstituteReturnValue(const Variable*) override {
    }

private:
    const Function* context_ = nullptr;
};

class ReturnValueOp : public BaseOperation<OpCodes::RETVALUE_OP> {
public:
    ReturnValueOp(const Function* context_func, const Value* value);

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Variable*> GetReturnValue() const override;
    void SubstituteReturnValue(const Variable* return_value) override;

private:
    const Function* context_ = nullptr;
    const Value* value_ = nullptr;
};

}  // namespace bier
