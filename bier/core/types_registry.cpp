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
#include "types_registry.h"

namespace bier {

DefaultTypesRegistry::DefaultTypesRegistry() {
    all_types_.insert(&i1_);
    all_types_.insert(&i8_);
    all_types_.insert(&i16_);
    all_types_.insert(&i32_);
    all_types_.insert(&i64_);

    all_types_.insert(&i1_ptr_);
    all_types_.insert(&i8_ptr_);
    all_types_.insert(&i16_ptr_);
    all_types_.insert(&i32_ptr_);
    all_types_.insert(&i64_ptr_);

    all_types_.insert(&any_ptr_);

    ptr_types_.insert(&i1_ptr_);
    ptr_types_.insert(&i8_ptr_);
    ptr_types_.insert(&i16_ptr_);
    ptr_types_.insert(&i32_ptr_);
    ptr_types_.insert(&i64_ptr_);

    type_to_ptr_.insert({&i1_, &i1_ptr_});
    type_to_ptr_.insert({&i8_, &i8_ptr_});
    type_to_ptr_.insert({&i16_, &i16_ptr_});
    type_to_ptr_.insert({&i32_, &i32_ptr_});
    type_to_ptr_.insert({&i64_, &i64_ptr_});

    ptr_types_.insert(&any_ptr_);
}

bool DefaultTypesRegistry::Has(const Type* type) const {
    return ContainerHas(all_types_, type);
}

const DefaultTypesRegistry* DefaultTypesRegistry::DefaultTypes() const {
    return this;
}

DefaultTypesRegistry* DefaultTypesRegistry::DefaultTypes() {
    return this;
}

const FunctionType* DefaultTypesRegistry::MakeFunctionType(std::optional<const Type*> return_type,
                                                    const std::vector<const Type*>& arguments) {
    auto funcType = std::make_unique<FunctionType>(return_type, arguments);
    const FunctionType* func_type_ptr = funcType.get();
    if (ContainerHas(function_types_, func_type_ptr)) {
        return function_types_.at(func_type_ptr).get();
    }

    function_types_.insert({ func_type_ptr, std::move(funcType) });
    all_types_.insert(func_type_ptr);
    ptr_types_.insert(func_type_ptr);
    func_types_.insert(func_type_ptr);
    return func_type_ptr;
}

bool DefaultTypesRegistry::IsPtrCompatibleWith(const Type* ptr_type, const Type* underlying_type) const {
    if (ptr_type == &any_ptr_) {
        return true;
    }
    if (ContainerHas(func_types_, ptr_type)) {
        return ptr_type == underlying_type;
    }

    const auto typed = dynamic_cast<const TypedPtrType*>(ptr_type);
    if (typed == nullptr) {
        return false;
    }
    return typed->GetUnderlying() == underlying_type;
}

const Type* DefaultTypesRegistry::GetPtrTo(const Type* type) {
    if (ContainerHas(type_to_ptr_, type)) {
        return type_to_ptr_.at(type);
    }
    const Type* ptr = custom_ptrs_.emplace(std::make_unique<TypedPtrType>(type)).first->get();
    ptr_types_.insert(ptr);
    all_types_.insert(ptr);
    type_to_ptr_.insert({type, ptr});
    return ptr;
}

}   // _bier
