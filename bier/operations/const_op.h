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
#include <bier/core/const_value.h>
#include <bier/operations/opcodes.h>

namespace bier {

class ConstOperation : public BaseOperation<OpCodes::CONST_OP> {
public:
    ConstOperation(const Function* context, const ConstValue* const_value, const Variable* ret_value);

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Variable*> GetReturnValue() const override;
    void SubstituteReturnValue(const Variable* return_value) override;

private:
    const Function* context_ = nullptr;
    const ConstValue* value_ = nullptr;
    const Variable* return_value_ = nullptr;
};

}  // namespace bier
