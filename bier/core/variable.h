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

#include <bier/core/value.h>

namespace bier {

class Variable;

using VariablePtr = std::unique_ptr<Variable>;

class Variable : public Value {
public:
    struct Metadata {
        std::string name;
        const Type* type = nullptr;
        bool is_mutable = false;

        Metadata(const std::string& _name, const Type* _type,
                 bool _is_mutable = false);
    };

    explicit Variable(const Metadata& metadata) : data_(metadata) {
    }

    // Value interface
    const Type* GetType() const override {
        return data_.type;
    }
    std::string GetName() const override {
        return data_.name;
    }
    bool IsMutable() const override {
        return data_.is_mutable;
    }

    void MakeImmutable() {
        data_.is_mutable = false;
    }

private:
    Metadata data_;
};

}  // namespace bier
