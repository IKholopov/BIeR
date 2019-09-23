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
#include <bier/core/int_types.h>
#include <bier/core/value.h>

namespace bier {

class ConstValue : public Value {
public:
    virtual std::string GetConstValue() const = 0;
};

class IntegerConst : public ConstValue {
public:
    IntegerConst(uint64_t value, const IntTypeBase* type);

    // Value interface
    const Type* GetType() const override {
        return type_;
    }
    std::string GetName() const override {
        return "";
    }
    bool IsMutable() const override {
        return false;
    }

    // ConstValue interface
    std::string GetConstValue() const override {
        return std::to_string(value_);
    }
    uint64_t GetValue() const {
        return value_;
    }
    const IntTypeBase* IntType() const {
        return type_;
    }

private:
    uint64_t value_ = 0;
    const IntTypeBase* type_ = nullptr;
};

}  // namespace bier
