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
#include "alloc_layout.h"

namespace bier {

AllocateLayout::AllocateLayout(const Function* context, const Layout* layout, const Value* count,
                               const Variable* result)
    : context_(context), layout_(layout), count_(count), result_ptr_(result) {
}

std::vector<const Value*> AllocateLayout::GetArguments() const {
    return {count_};
}

void AllocateLayout::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 1);
    count_ = args[0];
}

std::optional<const Variable*> AllocateLayout::GetReturnValue() const {
    return result_ptr_;
}

void AllocateLayout::SubstituteReturnValue(const Variable* return_value) {
    result_ptr_ = return_value;
}

}  // namespace bier
