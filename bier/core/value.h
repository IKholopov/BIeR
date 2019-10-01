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
#include <bier/core/type.h>

namespace bier {

class Value {
public:
    virtual ~Value() = default;
    virtual const Type* GetType() const = 0;
    virtual std::string GetName() const = 0;
    virtual bool IsMutable() const = 0;
    // Only for IMmutables
    virtual std::optional<const Operation*> GetOp() const = 0;
};

}  // namespace bier
