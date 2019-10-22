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
#include "module_builder.h"
#include <bier/core/exceptions.h>
#include <bier/operations/ops.h>

namespace bier {

ModuleBuilder::ModuleBuilder(Module* module_to_attach) : module_(module_to_attach) {
    assert(module_to_attach != nullptr);
}

Function* ModuleBuilder::CreateFunctionImpl(const std::string& name,
                                            std::optional<const Type*> return_type,
                                            const std::vector<const Type*>& arguments) {
    check(!module_->HasFunction(name), IRException("function " + name + " already defined!"));
    const FunctionType* func_type = module_->Types()->MakeFunctionType(return_type, arguments);
    return module_->AddFunction(name, func_type);
}

BasicBlock* ModuleBuilder::CreateBlockImpl(Function* function, const std::string& label,
                                       BasicBlock* insert_after) {
    current_block_ = function->CreateBlock(label, insert_after);
    return current_block_;
}

const StaticData* ModuleBuilder::CreateStaticDataImpl(const Layout* layout,
                                                      std::vector<ValuePtr>&& values,
                                                      const std::string& name) {
    StaticData* data = module_->AddStaticData(name, layout);
    for (size_t i = 0; i < values.size(); ++i) {
        data->SetEntry(std::move(values[i]), i);
    }
    return data;
}

const Variable* ModuleBuilder::CreateAddImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::ADD>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSubImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::SUB>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateMulImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::MULT>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSDivImpl(const Value* left, const Value* right,
                                       const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::SDIV>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSRemImpl(const Value* left, const Value* right,
                                       const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::SREM>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateEQImpl(const Value* left, const Value* right, const std::string& name,
                                     bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::EQ>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateNEImpl(const Value* left, const Value* right, const std::string& name,
                                     bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::NE>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSLEImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SLE>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSLTImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SLT>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSGEImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SGE>(left, right, name, is_mutable);
}

const Variable* ModuleBuilder::CreateSGTImpl(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SGT>(left, right, name, is_mutable);
}

void ModuleBuilder::CreateStoreImpl(const Value* ptr, const Value* value) {
    const Type* store_type = value->GetType();
    check(module_->Types()->IsPtr(ptr->GetType()),
          IRException("store to non-ptr type " + ptr->GetType()->ToString() + " requested",
                      CurrentFunction(), CurrentBlock()));
    check(module_->Types()->IsPtrCompatibleWith(ptr->GetType(), store_type),
          IRException("store to " + ptr->GetType()->ToString() + " of " +
                             store_type->ToString() + " is not possible",
                      CurrentFunction(), CurrentBlock()));
    CurrentBlock()->Append(std::make_unique<BinaryOperation>(
        CurrentFunction(), BinaryOperation::BinOp::STORE, value, ptr, nullptr));
}

const Value* ModuleBuilder::CreateAllocSingleImpl(const Type* alloc_type, const std::string& name,
                                        bool is_mutable) {
    return CreateAllocArrayImpl(alloc_type, CreateInt64Const(1), name, is_mutable);
}

const Value* ModuleBuilder::CreateAllocArrayImpl(const Type* alloc_type, const Value* count,
                                        const std::string& name, bool is_mutable) {
    const Variable* result =
        CurrentFunction()->AllocateVariable(Variable::Metadata(name, module_->Types()->GetPtrTo(alloc_type), is_mutable));
    check(module_->Types()->IsInteger(count->GetType()),
          IRException("cannot allocate count of " + count->GetType()->ToString() + " type",
                      CurrentFunction(), CurrentBlock()));
    CurrentBlock()->Append(std::make_unique<UnaryOperation>(
        CurrentFunction(), UnaryOperation::UnOp::ALLOC, count, result));

    return result;
}

const Value* ModuleBuilder::CreateAllocLayoutSingleImpl(const Layout* layout, const std::string& name,
                                        bool is_mutable) {
    return CreateAllocLayoutArrayImpl(layout, CreateInt64Const(1), name, is_mutable);
}

const Value* ModuleBuilder::CreateAllocLayoutArrayImpl(const Layout* layout, const Value* count,
                                        const std::string& name, bool is_mutable) {
    const Variable* result = CurrentFunction()->AllocateVariable(
        Variable::Metadata(name, module_->Types()->GetPtr(), is_mutable));
    CurrentBlock()->Append(
        std::make_unique<AllocateLayout>(CurrentFunction(), layout, count, result));
    return result;
}

const Variable* ModuleBuilder::CreateLoadImpl(const Value* ptr, const Type* load_type,
                                       const std::string& name, bool is_mutable) {
    const Variable* result =
        CurrentFunction()->AllocateVariable(Variable::Metadata(name, load_type, is_mutable));
    check(
        module_->Types()->IsPtr(ptr->GetType()),
        IRException("load from non-ptr type " + ptr->GetType()->ToString() + " requested",
                    CurrentFunction(), CurrentBlock()));
    check(module_->Types()->IsPtrCompatibleWith(ptr->GetType(), load_type),
          IRException("load from " + ptr->GetType()->ToString() + " to " +
                             load_type->ToString() + " is not possible",
                      CurrentFunction(), CurrentBlock()));
    CurrentBlock()->Append(std::make_unique<UnaryOperation>(
        CurrentFunction(), UnaryOperation::UnOp::LOAD, ptr, result));

    return result;
}

void ModuleBuilder::CreateAssignBetweenImpl(const Value* from, const Variable* to) {
    check(to->IsMutable(), IRException("cannot assign to immutable",
                                       CurrentFunction(), CurrentBlock()));
    check(from->GetType() == to->GetType(),
          IRException("cannot assign from type " + from->GetType()->ToString() + " to " +
                             to->GetType()->ToString() + " without explicit cast",
                      CurrentFunction(), CurrentBlock()));
    current_block_->Append(std::make_unique<UnaryOperation>(
        CurrentFunction(), UnaryOperation::UnOp::ASSIGN, from, to));
}

const Variable* ModuleBuilder::CreateAssignImpl(const Value* from, const std::string& name,
                                         bool is_mutable) {
    const Variable* result =
        CurrentFunction()->AllocateVariable(Variable::Metadata(name, from->GetType(), is_mutable));
    current_block_->Append(std::make_unique<UnaryOperation>(
        CurrentFunction(), UnaryOperation::UnOp::ASSIGN, from, result));
    return result;
}

std::optional<const Variable*> ModuleBuilder::CreateCallFuncImpl(const Function* func,
                                                      const std::vector<const Value*>& args,
                                                      const std::string& name, bool is_mutable) {
    std::optional<const Variable*> result;
    auto return_type = func->GetSignature()->ReturnType();
    if (return_type.has_value()) {
        result = CreateVariable(name, return_type.value(), is_mutable);
    }
    current_block_->Append(std::make_unique<CallOp>(CurrentFunction(), func, result, args));
    return result;
}

std::optional<const Variable*> ModuleBuilder::CreateCallFuncSigImpl(const FunctionSignature* func_sig,
                                                      const std::vector<const Value*>& args,
                                                      const std::string& name, bool is_mutable) {
    std::optional<const Variable*> result;
    auto return_type = func_sig->ReturnType();
    if (return_type.has_value()) {
        result = CreateVariable(name, return_type.value(), is_mutable);
    }
    current_block_->Append(
        std::make_unique<CallOp>(CurrentFunction(), func_sig->FuncType(), func_sig, result, args));
    return result;
}

std::optional<const Variable*> ModuleBuilder::CreateCallFuncPtr(const FunctionType* func_type, const Value* func_ptr,
                                                             const std::vector<const Value*>& args, const std::string& name,
                                                             bool is_mutable) {
    std::optional<const Variable*> result;
    auto return_type = func_type->ReturnType();
    if (return_type.has_value()) {
        result = CreateVariable(name, return_type.value(), is_mutable);
    }
    current_block_->Append(
        std::make_unique<CallOp>(CurrentFunction(), func_type, func_ptr, result, args));
    return result;
}

void ModuleBuilder::CreateReturnVoidImpl() {
    check(!CurrentFunction()->GetSignature()->FuncType()->ReturnType().has_value(),
          IRException(CurrentFunction()->GetName() +
                             " has return type, but void is returned",
                      CurrentFunction(), CurrentBlock()));
    current_block_->Append(std::make_unique<ReturnVoidOp>(CurrentFunction()));
}

void ModuleBuilder::CreateReturnValueImpl(const Value* value) {
    current_block_->Append(std::make_unique<ReturnValueOp>(CurrentFunction(), value));
}

const Variable* ModuleBuilder::CreateGEPImpl(const Value* ptr, const Layout* layout, int element_index,
                                      const std::string& name, bool is_mutable,
                                      std::optional<const Value*> base_offset,
                                      std::optional<const Value*> element_offset) {
    assert(layout != nullptr);
    check(module_->Types()->IsPtr(ptr->GetType()),
          IRException("cannot load from type " + ptr->GetType()->ToString(),
                      CurrentFunction(), CurrentBlock()));
    const Variable* result = CreateVariable(
        name, module_->Types()->GetPtrTo(layout->GetEntry(element_index)), is_mutable);
    auto gep = std::make_unique<GEPOp>(CurrentFunction(), ptr, element_index, result, layout,
                                       base_offset, element_offset);
    current_block_->Append(std::move(gep));
    return result;
}

const Variable* ModuleBuilder::CastToImpl(const Value* value, const Type* target_type,
                                   const std::string& name, bool is_mutable) {
    const Variable* result = CreateVariable(name, target_type, is_mutable);
    current_block_->Append(std::make_unique<CastOperation>(CurrentFunction(), value, result));
    return result;
}

Function* ModuleBuilder::CurrentFunction() {
    assert(current_block_ != nullptr);
    return module_->GetFunction(current_block_->GetContextFunction()->GetName());
}

void ModuleBuilder::CreateBranchImpl(const BasicBlock* target) {
    current_block_->Append(std::make_unique<BranchOperation>(CurrentFunction(), target));
    current_block_->TerminateBlock();
}

void ModuleBuilder::CreateConditionBranchImpl(const Value* condtion, const BasicBlock* target_true,
                                          const BasicBlock* target_false) {
    check(condtion->GetType() == module_->Types()->GetInt1(),
          IRException("condition should be of type bool", CurrentFunction(), CurrentBlock()));
    current_block_->Append(std::make_unique<ConditionalBranchOperation>(CurrentFunction(), condtion,
                                                                        target_true, target_false));
    current_block_->TerminateBlock();
}

void ModuleBuilder::AttachTo(BasicBlock* block) {
    assert(block != nullptr);
    current_block_ = block;
}

const Variable* ModuleBuilder::CreateVariable(const std::string& name, const Type* type,
                                              bool is_mutable) {

    return CurrentFunction()->AllocateVariable(Variable::Metadata(name, type, is_mutable));
}

template <BinaryOperation::BinOp op>
const Variable* ModuleBuilder::CreateCmp(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    const Variable* result = CreateVariable(name, module_->Types()->GetInt1(), is_mutable);
    current_block_->Append(
        std::make_unique<BinaryOperation>(CurrentFunction(), op, left, right, result));
    return result;
}

template <BinaryOperation::BinOp op>
const Variable* ModuleBuilder::CreateArithmetic(const Value* left, const Value* right,
                                             const std::string& name, bool is_mutable) {
    check(left->GetType() == right->GetType(),
          IRException("types mismatch " + left->GetType()->ToString() + " " +
                             right->GetType()->ToString(),
                      CurrentFunction(), CurrentBlock()));
    const Variable* result = CreateVariable(name, left->GetType(), is_mutable);
    current_block_->Append(
        std::make_unique<BinaryOperation>(CurrentFunction(), op, left, right, result));
    return result;
}

}  // namespace bier
