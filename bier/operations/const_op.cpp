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
#include "const_op.h"

namespace bier {

ConstOperation::ConstOperation(const Function* context, const ConstValue* const_value,
                               const Variable* ret_value)
    : context_(context), value_(const_value), return_value_(ret_value) {
}

const Function* ConstOperation::GetContextFunction() const {
    return context_;
}

std::vector<const Value*> ConstOperation::GetArguments() const {
    return {value_};
}

void ConstOperation::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 1);
    auto new_arg = dynamic_cast<const ConstValue*>(args[0]);
    assert(new_arg != nullptr);
    value_ = new_arg;
}

std::optional<const Variable*> ConstOperation::GetReturnValue() const {
    return {return_value_};
}

void ConstOperation::SubstituteReturnValue(const Variable* return_value) {
    return_value_ = return_value;
}

}  // namespace bier
