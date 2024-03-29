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
#include "variable_name.h"
#include <bier/core/exceptions.h>

namespace bier {

std::string VariableNameStorage::Allocate(const std::string& name, bool createNew) {
    if (name.empty()) {
        return std::to_string(anonymous_counter_++);
    }
    if (ContainerHas(name_to_id, name)) {
        if (createNew) {
            return name + std::to_string(name_to_id.at(name)++);
        }
        return name;
    }
    name_to_id.insert({name, 0});
    return name;
}

std::string VariableNameStorage::AllocateUnique(const std::string& name) {
    check(!ContainerHas(name_to_id, name),
          IRException("Failed to allocate unique variable " + name, context_));
    name_to_id.insert({name, 0});
    return name;
}

const Function* VariableNameStorage::GetContextFunction() const {
    return context_;
}

}  // namespace bier
