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
#include "operation.h"
#include <cassert>

namespace bier {

BinaryOperation::BinaryOperation(const Function* context_func,
                                 BinaryOperation::BinOp op,
                                 Value* left, Value* right, Value* return_value)
    : context_function_(context_func),
      left_value_(left),
      right_value_(right),
      return_value_(return_value),
      op_(op) {
    assert(context_func != nullptr);
    assert(left != nullptr);
    assert(right != nullptr);
    assert(return_value != nullptr);
    assert(op != BinOp::INVALID);
}

const Function* BinaryOperation::GetContextFunction() const {
    return context_function_;
}

std::vector<Value*> BinaryOperation::GetArguments() const {
    return {left_value_, right_value_};
}

Value* BinaryOperation::GetReturnValue() const {
    return return_value_;
}

UnaryOpertaion::UnaryOpertaion(const Function* context_func, UnaryOpertaion::UnOp op,
                               Value* argument, Value* return_value)
    : context_function_(context_func),
      argument_(argument),
      return_value_(return_value),
      op_(op)
{
    assert(context_func != nullptr);
    assert(argument != nullptr);
    assert(return_value != nullptr);
    assert(op != UnOp::INVALID);
}

const Function* UnaryOpertaion::GetContextFunction() const
{
    return context_function_;
}

std::vector<Value*> UnaryOpertaion::GetArguments() const
{
    return { argument_ };
}

Value* UnaryOpertaion::GetReturnValue() const
{
    return return_value_;
}



}   // _bier
