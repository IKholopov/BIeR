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
#include <bier/core/basic_types.h>
#include <bier/core/type.h>

namespace bier {

class IntTypeBase : public Type {
public:
    bool IsValid(int64_t value) const {
        return IsValid(static_cast<uint64_t>(value));
    }
    virtual bool IsValid(uint64_t value) const = 0;
    virtual unsigned int GetNBits() const = 0;
};

template <int NBits>
class IntType : public IntTypeBase {
public:
    // Type interface
    std::string ToString() const override {
        return "i" + std::to_string(NBits);
    }

    bool IsValid(uint64_t value) const override {
        static_assert (NBits <= 64, "values over 64-bits length are not supported");
        return ((~( NBits == 64 ? -1 : ((1ull << NBits) - 1ull)) & value) == 0);
    }

    unsigned int GetNBits() const override {
        return NBits;
    }
};

}   // _bier
