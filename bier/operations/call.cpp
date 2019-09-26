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
#include "call.h"
#include <bier/core/exceptions.h>

namespace bier {

CallOp::CallOp(const Function* context, const Function* function,
               std::optional<const Value*> return_value, const std::vector<const Value*>& arguments)
    : args_(arguments),
      context_(context),
      type_(function->GetSignature()->FuncType()),
      value_(function),
      return_value_(return_value) {
    check((!return_value_.has_value() && !function->GetSignature()->ReturnType().has_value()) ||
              return_value.value()->GetType() == function->GetSignature()->ReturnType().value(),
          IRException("call to function " + function->GetName() +
                             " does not match return type", context_));
    int index = 0;
    check(function->GetSignature()->Arguments().Size() == arguments.size(),
          IRException("not enough arguments", context_));
    for (const auto arg : function->GetSignature()->Arguments()) {
        check(arg->GetType() == arguments[index++]->GetType(),
              IRException("type for argument " + arg->GetName() + " does not match",
                          context_));
    }
}

CallOp::CallOp(const Function* context, const FunctionType* type, const Value* func_value,
               std::optional<const Value*> return_value, const std::vector<const Value*>& arguments)
    : args_(arguments),
      context_(context),
      type_(type),
      value_(func_value),
      return_value_(return_value) {
    CheckReturnAndArgs();
}

std::vector<const Value*> CallOp::GetArguments() const {
    std::vector<const Value*> args = {value_};
    args.insert(args.end(), args_.begin(), args_.end());
    return args;
}

void CallOp::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == args_.size() + 1);
    value_ = *args.begin();
    args_ = std::vector(args.begin() + 1, args.end());

}

void CallOp::SubstituteReturnValue(const Value* return_value) {
    assert(return_value_.has_value());
    return_value_ = return_value;
}

void CallOp::CheckReturnAndArgs() const {
    auto message = "call to function does not match return type "
            + (!return_value_.has_value() ? "none" : return_value_.value()->GetType()->ToString())
            + " vs " + (!type_->ReturnType().has_value() ? "none" : type_->ReturnType().value()->ToString());
    check((!return_value_.has_value() && !type_->ReturnType().has_value()) ||
              (return_value_.has_value() &&
               return_value_.value()->GetType() == type_->ReturnType().value()),
          IRException(message, context_));
    int index = 0;
    check(type_->Arguments().size() == args_.size(), IRException("not enough arguments", context_));
    for (const auto arg : type_->Arguments()) {
        check(arg == args_[index]->GetType(),
            IRException("type for argument " + args_[index]->GetName() + " does not match",
                        context_));
        index += 1;
    }
}

}  // namespace bier
