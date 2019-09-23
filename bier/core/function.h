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
#include <bier/common.h>
#include <bier/core/basic_block.h>
#include <bier/core/type.h>
#include <bier/core/value.h>
#include <bier/core/variable.h>
#include <bier/core/variable_name.h>
#include <bier/utils/iterator_range.h>
#include <bier/utils/intrusive_list.h>
#include <bier/utils/ptr_iterator.h>

#include <optional>
#include <vector>

namespace bier {

class FunctionType : public Type {
public:
    struct HashPtr {
        HashType operator()(const FunctionType* type) const;
    };
    struct PredPtr {
        constexpr bool operator()(const FunctionType* left, const FunctionType* right) const {
            return left->return_type_ == right->return_type_ &&
                   left->arguments_ == right->arguments_;
        }
    };

    explicit FunctionType(std::optional<const Type*> return_type = std::nullopt,
                          const std::vector<const Type*>& arguments = {});

    std::string ToString() const override;

    const std::vector<const Type*>& Arguments() const {
        return arguments_;
    }
    const std::optional<const Type*> ReturnType() const {
        return return_type_;
    }

private:
    std::vector<const Type*> arguments_;
    std::optional<const Type*> return_type_ = std::nullopt;
};

struct ArgumentData {
    const Type* const type;
    std::string name;

    explicit ArgumentData(const Type* typeArg) : type(typeArg) {
    }
};

class ArgumentValue : public Value {
public:
    explicit ArgumentValue(const Type* argType) : data_(std::make_unique<ArgumentData>(argType)) {
    }

    // Value interface
    const Type* GetType() const override;
    std::string GetName() const override;
    bool IsMutable() const override {
        return false;
    }

    void SetName(const std::string& name) {
        data_->name = name;
    }

private:
    std::unique_ptr<ArgumentData> data_;
};

using ArgValuePtr = std::unique_ptr<ArgumentValue>;

class FunctionSignature : public Value {
public:
    struct HashPtr {
        HashType operator()(const FunctionSignature* signature) const;
    };
    struct PredPtr {
        constexpr bool operator()(const FunctionSignature* left,
                                  const FunctionSignature* right) const {
            return left->type_ == right->type_ && left->name_ == right->name_;
        }
    };

    FunctionSignature(const std::string& name, const FunctionType* type);

    std::string ToString() const;
    const FunctionType* FuncType() const {
        return type_;
    }
    const std::string& Name() const {
        return name_;
    }

    // Value interface
    const Type* GetType() const override;
    std::string GetName() const override;
    bool IsMutable() const override;

    std::optional<const Type*> ReturnType() const {
        return type_->ReturnType();
    }
    auto Arguments() const {
        return IteratorPtrRange<decltype(arguments_)>(arguments_);
    }

    auto Arguments() {
        return MutableIteratorPtrRange<decltype(arguments_)>(arguments_);
    }

private:
    std::vector<ArgValuePtr> arguments_;
    const FunctionType* type_;
    std::string name_;
};

class Function : public Value {
public:
    explicit Function(FunctionSignature* signature) : signature_(signature) {
        assert(signature != nullptr);
    }

    // Value function
    const Type* GetType() const override;
    std::string GetName() const override;
    bool IsMutable() const override {
        return false;
    }

    const FunctionSignature* GetSignature() const {
        return signature_;
    }
    FunctionSignature* GetSignature() {
        return signature_;
    }

    BasicBlock* CreateBlock(const std::string& label = "", BasicBlock* insertAfter = nullptr);
    BasicBlock* CreateBlockAtStart(const std::string& label = "");
    OneWayIteratorRange<BasicBlock> GetBlocks() const;
    OneWayIteratorRange<BasicBlock> GetBlocks();

    const Variable* AllocateVariable(const Variable::Metadata& metadata);

    auto GetVariables() const {
        return IteratorRange(variables_);
    }

    void Normalize();

private:
    StdHashMap<std::string, VariablePtr> variables_;
    VariableNameStorage variable_names_;
    BasicBlockPtr first_block_;
    BasicBlock* last_block_ = nullptr;
    FunctionSignature* signature_ = nullptr;

    void AllocateArgumentVariables();
    const Variable* AllocateUnique(const Variable::Metadata& metadata);
    void ClearLostVariables();
};

}  // namespace bier
