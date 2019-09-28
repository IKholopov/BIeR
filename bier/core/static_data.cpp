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
#include "static_data.h"
#include <bier/core/exceptions.h>

namespace bier {

StaticData::StaticData(DefaultTypesRegistry* types, const Layout* layout, const std::string& name)
    : layout_(layout),
      name_(name),
      ptrType_(types->GetPtr()) {
    values_.resize(layout_->Entries().Size());
}

const Type* StaticData::GetType() const {
    return ptrType_;
}

std::string StaticData::GetName() const {
    return name_;
}

bool StaticData::IsMutable() const {
    return false;
}

const Value* StaticData::GetEntry(size_t index) const {
    check(index < values_.size(), IRException("index " + std::to_string(index)
                                                           + " is out of static data bounds"));
    return values_.at(index).get();
}

void StaticData::SetEntry(ValuePtr&& value, size_t index) {
    check(index < values_.size(), IRException("index " + std::to_string(index)
                                                           + " is out of static data bounds"));
    values_.insert(values_.begin() + index, std::move(value));
}

}   // bier
