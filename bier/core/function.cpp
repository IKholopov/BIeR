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
#include "function.h"
#include <boost/functional/hash.hpp>

namespace bier {

HashType FunctionType::HashPtr::operator()(const FunctionType* type) const {
    assert(type != nullptr);
    HashType hash = 0;
    boost::hash_combine(hash, type->return_type_.has_value());
    if (type->return_type_.has_value()) {
        boost::hash_combine(hash, type->return_type_.value());
    }
    for (const Type* arg : type->arguments_) {
        boost::hash_combine(hash, arg);
    }
    return hash;
}

FunctionType::FunctionType(std::optional<const Type*> return_type,
                           const std::vector<const Type*>& arguments)
    : arguments_(arguments), return_type_(return_type) {
}

std::string FunctionType::ToString() const {
    return "func()";
}

HashType FunctionSignature::HashPtr::operator()(const FunctionSignature* signature) const {
    assert(signature != nullptr);
    HashType hash = 0;
    boost::hash_combine(hash, signature->FuncType());
    boost::hash_combine(hash, signature->Name());
    return hash;
}

FunctionSignature::FunctionSignature(const std::string& name, const FunctionType* type)
    : type_(type), name_(name) {
    for (const Type* arg : type->Arguments()) {
        arguments_.push_back(std::make_unique<ArgumentValue>(arg));
    }
}

std::string FunctionSignature::ToString() const {
    return "func()";
}

const Type* FunctionSignature::GetType() const {
    return FuncType();
}

std::string FunctionSignature::GetName() const {
    return Name();
}

bool FunctionSignature::IsMutable() const {
    return false;
}

const Type* Function::GetType() const {
    return signature_->FuncType();
}

std::string Function::GetName() const {
    return signature_->Name();
}

BasicBlock* Function::CreateBlock(const std::string& label, BasicBlock* insertAfter) {
    if (insertAfter == nullptr) {
        insertAfter = last_block_;
    }

    auto block = std::make_unique<BasicBlock>(this, label);

    if (insertAfter == nullptr) {
        AllocateArgumentVariables();
        first_block_ = std::move(block);
        last_block_ = first_block_.get();
        return last_block_;
    }
    insertAfter->InsertNext(std::move(block));
    if (last_block_ == insertAfter) {
        last_block_ = last_block_->Next();
    }
    return insertAfter->Next();
}

BasicBlock* Function::CreateBlockAtStart(const std::string& label) {
    auto block = std::make_unique<BasicBlock>(this, label);
    block->Attach(std::move(first_block_));
    first_block_ = std::move(block);

    return first_block_.get();
}

OneWayIteratorRange<BasicBlock> Function::GetBlocks() const {
    return OneWayIteratorRange<BasicBlock>(first_block_.get());
}

OneWayIteratorRange<BasicBlock> Function::GetBlocks() {
    return OneWayIteratorRange<BasicBlock>(first_block_.get());
}

const Variable* Function::AllocateVariable(const Variable::Metadata& metadata) {
    check(!metadata.is_mutable || !metadata.name.empty(), std::runtime_error("mutable variable should have a name"));
    Variable::Metadata data = metadata;
    const bool create_new = !data.is_mutable;
    data.name = variable_names_.Allocate(data.name, create_new);
    if (ContainerHas(variables_, data.name)) {
        return variables_.at(data.name).get();
    }
    auto var = std::make_unique<Variable>(data);
    const Variable* varPtr = var.get();
    variables_.insert({ varPtr->GetName(), std::move(var) });
    return varPtr;
}

void Function::Normalize() {
    ClearLostVariables();
}

void Function::AllocateArgumentVariables() {
    for (const auto arg : signature_->Arguments()) {
        check(!arg->GetName().empty(), std::runtime_error("please, name your argument variables"));
        AllocateUnique(Variable::Metadata(arg->GetName(), arg->GetType()));
    }
}

const Variable* Function::AllocateUnique(const Variable::Metadata& metadata) {
    variable_names_.AllocateUnique(metadata.name);
    auto var = std::make_unique<Variable>(metadata);
    const Variable* varPtr = var.get();
    variables_.insert({varPtr->GetName(), std::move(var)});
    return varPtr;
}

void Function::ClearLostVariables() {
    StdHashSet<const Value*> values;
    for (const auto& block : GetBlocks()) {
        for (const auto& op : block.GetOperations()) {
            for (const Value* value : op->GetArguments()) {
                values.insert(value);
            }
            if (op->GetReturnValue().has_value()) {
                values.insert(op->GetReturnValue().value());
            }
        }
    }

    std::vector<std::string> varsToDelete;
    for (const auto& [name, var] : variables_) {
        if (!ContainerHas(values, var.get())) {
            varsToDelete.push_back(name);
        }
    }

    for (const auto& name : varsToDelete) {
        variables_.erase(name);
    }
}

const Type* ArgumentValue::GetType() const {
    return data_->type;
}

std::string ArgumentValue::GetName() const {
    return data_->name;
}

}  // namespace bier
