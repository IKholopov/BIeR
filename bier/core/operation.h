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
#include <bier/core/value.h>

namespace bier {

class Operation : public FunctionContextMemeber {
public:
    virtual std::vector<Value*> GetArguments() const = 0;
    virtual Value* GetReturnValue() const = 0;
};

class BinaryOperation : public Operation {
public:
    enum class BinOp {
        ADD,
        SUB,
        MULT,
        DIV,
        EQ,
        NE,
        LE,
        LT,
        GE,
        GT,
        STORE,
        INVALID
    };

    BinaryOperation(const Function* context_func, BinOp op, Value* left, Value* right, Value* return_value);

    BinOp GetOp() const {
        return op_;
    }

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<Value*> GetArguments() const override;
    Value* GetReturnValue() const override;

private:
    const Function* context_function_ = nullptr;
    Value* left_value_ = nullptr;
    Value* right_value_ = nullptr;
    Value* return_value_ = nullptr;
    BinOp op_ = BinOp::INVALID;
};

class UnaryOpertaion : public Operation {
public:
    enum class UnOp {
        ALLOC,
        LOAD,
        INVALID
    };

    UnaryOpertaion(const Function* context_func, UnOp op, Value* argument, Value* return_value);

    UnOp GetOp() const {
        return op_;
    }

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<Value*> GetArguments() const override;
    Value* GetReturnValue() const override;

private:
    const Function* context_function_ = nullptr;
    Value* argument_ = nullptr;
    Value* return_value_ = nullptr;
    UnOp op_ = UnOp::INVALID;
};

}   // _bier
