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

#include <bier/core/function_context.h>
#include <bier/core/variable.h>
#include <vector>

namespace bier {

class Operation : public FunctionContextMemeber {
public:
    virtual std::vector<const Value*> GetArguments() const = 0;
    virtual void SubstituteArguments(const std::vector<const Value*>& args) = 0;

    virtual std::optional<const Variable*> GetReturnValue() const = 0;
    virtual void SubstituteReturnValue(const Variable* return_value) = 0;

    virtual int OpCode() const = 0;
};

template <int IOpCode>
class BaseOperation : public Operation {
    int OpCode() const override {
        return IOpCode;
    }
};

class BinaryOperation : public Operation {
public:
    enum class BinOp {
        ADD,    // a + b
        SUB,    // a - b
        MULT,   // a * b
        UDIV,   // a / b
        SDIV,   // a / b
        UREM,   // a % b
        SREM,   // a % b
        EQ,     // a == b
        NE,     // a != b
        SLE,    // a <= b
        SLT,    // a < b
        SGE,    // a >= b
        SGT,    // a > b
        STORE,  // *b = a
        INVALID
    };

    BinaryOperation(const Function* context_func, BinOp op, const Value* left, const Value* right,
                    const Variable* return_value);

    BinOp GetOp() const {
        return op_;
    }

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Variable*> GetReturnValue() const override;
    void SubstituteReturnValue(const Variable* return_value) override;
    int OpCode() const override;

    const Value* LeftValue() const {
        return left_value_;
    }
    const Value* RightValue() const {
        return right_value_;
    }

private:
    const Function* context_function_ = nullptr;
    const Value* left_value_ = nullptr;
    const Value* right_value_ = nullptr;
    const Variable* return_value_ = nullptr;
    BinOp op_ = BinOp::INVALID;
};

class UnaryOperation : public Operation {
public:
    enum class UnOp { ALLOC, LOAD, ASSIGN, INVALID };

    UnaryOperation(const Function* context_func, UnOp op, const Value* argument,
                   const Variable* return_value);

    UnOp GetOp() const {
        return op_;
    }

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Variable*> GetReturnValue() const override;
    void SubstituteReturnValue(const Variable* return_value) override;
    int OpCode() const override;

private:
    const Function* context_function_ = nullptr;
    const Value* argument_ = nullptr;
    const Variable* return_value_ = nullptr;
    UnOp op_ = UnOp::INVALID;
};

}  // namespace bier
