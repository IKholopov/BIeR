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
#include <bier/core/module.h>
#include <bier/core/operation.h>
#include <ostream>

namespace bier {

class StringSerializer {
public:
    std::ostream& TranslateFunctionSignature(const FunctionSignature* op,
                                             std::ostream& stream) const;
    std::ostream& TranslateOp(const Operation* op, std::ostream& stream) const;
    std::ostream& TranslateValue(const Value* value, std::ostream& stream) const;
    std::ostream& TranslateLayout(const Layout* layout, std::ostream& stream) const;
    std::ostream& PrintModule(const Module* module, std::ostream& stream) const;

    virtual std::string OpCodeName(int code) const;
};

}  // namespace bier
