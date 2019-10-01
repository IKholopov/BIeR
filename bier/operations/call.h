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
#include <bier/operations/opcodes.h>

namespace bier {

class CallOp : public BaseOperation<OpCodes::Op::CALL_OP> {
public:
    CallOp(const Function* context, const Function* function,
           std::optional<const Variable*> return_value = std::nullopt,
           const std::vector<const Value*>& arguments = {});
    CallOp(const Function* context, const FunctionType* type, const Value* func_value,
           std::optional<const Variable*> return_value = std::nullopt,
           const std::vector<const Value*>& arguments = {});

    // FunctionContextInterface
    const Function* GetContextFunction() const override {
        return context_;
    }

    // Operation Interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Variable*> GetReturnValue() const override {
        return return_value_;
    }
    void SubstituteReturnValue(const Variable* return_value) override;

    const Value* Callee() const {
        return value_;
    }
    const FunctionType* FuncType() const {
        return type_;
    }

private:
    std::vector<const Value*> args_;
    const Function* context_ = nullptr;
    const FunctionType* type_ = nullptr;
    const Value* value_ = nullptr;
    std::optional<const Variable*> return_value_ = std::nullopt;

    void CheckReturnAndArgs() const;
};

}  // namespace bier
