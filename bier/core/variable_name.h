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
#include <string>

namespace bier {

class VariableNameStorage : public FunctionContextMemeber {
public:
    explicit VariableNameStorage(const Function* function) : context_(function) {
    }

    std::string Allocate(const std::string& name, bool createNew = false);
    std::string AllocateUnique(const std::string& name);

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

private:
    StdHashMap<std::string, int> name_to_id;
    const Function* context_ = nullptr;
    int anonymous_counter_ = 0;

};

}  // namespace bier
