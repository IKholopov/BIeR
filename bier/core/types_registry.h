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
#include <bier/core/basic_types.h>
#include <bier/core/function.h>

#include <vector>
#include <unordered_map>

namespace bier {

class DefaultTypesRegistry;

class TypeRegistryInterface {
public:
    virtual ~TypeRegistryInterface() = default;
    virtual bool Has(const Type* type) const = 0;
    virtual const DefaultTypesRegistry* DefaultTypes() const  = 0;
    virtual DefaultTypesRegistry* DefaultTypes() = 0;
};

class DefaultTypesRegistry : public TypeRegistryInterface {
public:
    DefaultTypesRegistry();

    // TypeRegistryInterface interface
    bool Has(const Type* type) const override;
    const DefaultTypesRegistry* DefaultTypes() const override;
    DefaultTypesRegistry* DefaultTypes() override;

    const Type* GetInt1() const {
        return &i1_;
    }
    const Type* GetInt8() const {
        return &i8_;
    }
    const Type* GetInt16() const {
        return &i16_;
    }
    const Type* GetInt32() const {
        return &i32_;
    }
    const Type* GetInt64() const {
        return &i64_;
    }

    const Type* GetInt1Ptr() const {
        return &i1_ptr_;
    }
    const Type* GetInt8Ptr() const {
        return &i8_ptr_;
    }
    const Type* GetInt16Ptr() const {
        return &i16_ptr_;
    }
    const Type* GetInt32Ptr() const {
        return &i32_ptr_;
    }
    const Type* GetInt64Ptr() const {
        return &i64_ptr_;
    }

    const Type* GetPtr() const {
        return &any_ptr_;
    }

    const FunctionType* MakeFunctionType(std::optional<const Type*> return_type = std::nullopt,
                                 const std::vector<const Type*>& arguments = {});

    bool IsPtr(const Type* type) const {
        return ContainerHas(ptr_types_, type);
    }
    bool IsPtrCompatibleWith(const Type* ptr_type, const Type* underlying_type) const;
    bool IsInteger(const Type* type) const {
        return dynamic_cast<const IntTypeBase*>(type) != nullptr;
    }
    const Type* GetPtrTo(const Type* type);

private:
    IntType<1> i1_;
    IntType<8> i8_;
    IntType<16> i16_;
    IntType<32> i32_;
    IntType<64> i64_;

    TypedPtrType i1_ptr_ = TypedPtrType(&i1_);
    TypedPtrType i8_ptr_ = TypedPtrType(&i8_);
    TypedPtrType i16_ptr_ = TypedPtrType(&i16_);
    TypedPtrType i32_ptr_ = TypedPtrType(&i32_);
    TypedPtrType i64_ptr_ = TypedPtrType(&i64_);

    PtrType any_ptr_;

    HashPtrMap<FunctionType, std::unique_ptr<FunctionType>> function_types_;
    StdHashSet<const Type*> func_types_;
    StdHashSet<const Type*> all_types_;
    StdHashSet<const Type*> ptr_types_;
    StdHashMap<const Type*, const Type*> type_to_ptr_;
    StdHashSet<TypePtr> custom_ptrs_;
};

}   // _bier
