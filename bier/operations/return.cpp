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
#include "return.h"
#include <bier/core/exceptions.h>

namespace bier {

const Function* ReturnVoidOp::GetContextFunction() const {
    return context_;
}

std::vector<const Value*> ReturnVoidOp::GetArguments() const {
    return {};
}

std::optional<const Value*> ReturnVoidOp::GetReturnValue() const {
    return std::nullopt;
}

ReturnValueOp::ReturnValueOp(const Function* context_func, const Value* value)
    : context_(context_func), value_(value) {
    assert(context_func != nullptr);
    assert(value != nullptr);
    auto return_type = context_->GetSignature()->FuncType()->ReturnType();
    check(return_type.has_value(),
          IRException(context_->GetName() + " has return type, but void is returned",
                      context_));
    check(return_type.value() == value->GetType(),
          IRException(context_->GetName() + " has return type " +
                             return_type.value()->ToString() + ", but " +
                             value->GetType()->ToString() + " is returned",
                      context_));
}

const Function* ReturnValueOp::GetContextFunction() const {
    return context_;
}

std::vector<const Value*> ReturnValueOp::GetArguments() const {
    return {value_};
}

void ReturnValueOp::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 1);
    value_ = args[0];
}

std::optional<const Value*> ReturnValueOp::GetReturnValue() const {
    return std::nullopt;
}

void ReturnValueOp::SubstituteReturnValue(const Value*) {
    assert(false);
}

}  // namespace bier
