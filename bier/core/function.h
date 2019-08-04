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
#include <bier/common.h>

#include <bier/core/value.h>
#include <bier/core/type.h>

#include <optional>
#include <vector>

namespace bier {

class FunctionSignature : public Type {
public:
    FunctionSignature(const std::string& name, Type* return_type = nullptr,
             std::vector<ValuePtr>&& arguments = {});

private:
    std::vector<ValuePtr> arguments_;
    std::string name_;
    std::optional<Type*> return_type_ = std::nullopt;
};

class Function : public Value {
public:
    explicit Function(const FunctionSignature* signature) : signature_(signature) {
        assert(signature != nullptr);
    }

    // Value function
    const Type* GetType() const override;

private:
    std::vector<BasicBlockPtr> blocks_;
    const FunctionSignature* signature_ = nullptr;
};

class FunctionContextMemeber {
public:
    virtual ~FunctionContextMemeber() = default;
    virtual const Function* GetContextFunction() const = 0;
};

}   // _bier
