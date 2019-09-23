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
#include <bier/operations/opcodes.h>
#include <cassert>

namespace bier {

BinaryOperation::BinaryOperation(const Function* context_func, BinaryOperation::BinOp op,
                                 const Value* left, const Value* right, const Value* return_value)
    : context_function_(context_func),
      left_value_(left),
      right_value_(right),
      return_value_(return_value),
      op_(op) {
    assert(context_func != nullptr);
    assert(left != nullptr);
    assert(right != nullptr);
    assert(return_value != nullptr || op == BinOp::STORE);
    assert(op != BinOp::INVALID);
}

const Function* BinaryOperation::GetContextFunction() const {
    return context_function_;
}

std::vector<const Value*> BinaryOperation::GetArguments() const {
    return {left_value_, right_value_};
}

void BinaryOperation::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 2);
    left_value_ = args[0];
    right_value_ = args[1];
}

std::optional<const Value*> BinaryOperation::GetReturnValue() const {
    return return_value_ == nullptr ? std::nullopt : std::make_optional(return_value_);
}

void BinaryOperation::SubstituteReturnValue(const Value* return_value) {
    assert(GetReturnValue().has_value());
    return_value_ = return_value;
}

int BinaryOperation::OpCode() const {
    static_assert(OpCodes::ADD_OP == 0, "CodeOps for BinOps changed");
    static_assert(OpCodes::STORE_OP + 1 == static_cast<int>(BinOp::INVALID),
                  "CodeOps for BinOps changed");
    static_assert(static_cast<int>(BinOp::ADD) == 0, "New BinOp not handled");
    static_assert(static_cast<int>(BinOp::INVALID) == 14, "New BinOp not handled");

    return static_cast<int>(OpCodes::ADD_OP) + static_cast<int>(op_);
}

UnaryOperation::UnaryOperation(const Function* context_func, UnaryOperation::UnOp op,
                               const Value* argument, const Value* return_value)
    : context_function_(context_func), argument_(argument), return_value_(return_value), op_(op) {
    assert(context_func != nullptr);
    assert(argument != nullptr);
    assert(return_value != nullptr);
    assert(op != UnOp::INVALID);
}

const Function* UnaryOperation::GetContextFunction() const {
    return context_function_;
}

std::vector<const Value*> UnaryOperation::GetArguments() const {
    return {argument_};
}

void UnaryOperation::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 1);
    argument_ = args.front();
}

std::optional<const Value*> UnaryOperation::GetReturnValue() const {
    return return_value_;
}

void UnaryOperation::SubstituteReturnValue(const Value* return_value) {
    return_value_ = return_value;
}

int UnaryOperation::OpCode() const {
    static_assert(OpCodes::ALLOC_OP == 14, "CodeOps for UnOps changed");
    static_assert(OpCodes::ASSIGN_OP + 1 == static_cast<int>(UnOp::INVALID) + OpCodes::ALLOC_OP,
                  "CodeOps for UnOps changed");
    static_assert(static_cast<int>(UnOp::ALLOC) == 0, "New UnOp not handled");
    static_assert(static_cast<int>(UnOp::INVALID) == 3, "New UnOp not handled");

    return static_cast<int>(OpCodes::ALLOC_OP) + static_cast<int>(op_);
}

}  // namespace bier
