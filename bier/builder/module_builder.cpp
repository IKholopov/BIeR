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

Function* ModuleBuilder::CreateFunction(const std::string& name,
                                        std::optional<const Type*> return_type,
                                        const std::vector<const Type*>& arguments) {
    check(!module_->HasFunction(name), IRException("function " + name + " already defined!"));
    const FunctionType* func_type = module_->Types()->MakeFunctionType(return_type, arguments);
    return module_->AddFunction(name, func_type);
}

BasicBlock* ModuleBuilder::CreateBlock(Function* function, const std::string& label,
                                       BasicBlock* insert_after) {
    current_block_ = function->CreateBlock(label, insert_after);
    return current_block_;
}

const Value* ModuleBuilder::CreateAdd(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::ADD>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateSub(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::SUB>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateMul(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::MULT>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateSDiv(const Value* left, const Value* right,
                                       const std::string& name, bool is_mutable) {
    return CreateArithmetic<BinaryOperation::BinOp::SDIV>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateEQ(const Value* left, const Value* right, const std::string& name,
                                     bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::EQ>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateNE(const Value* left, const Value* right, const std::string& name,
                                     bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::NE>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateSLE(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SLE>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateSLT(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SLT>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateSGE(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SGE>(left, right, name, is_mutable);
}

const Value* ModuleBuilder::CreateSGT(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    return CreateCmp<BinaryOperation::BinOp::SGT>(left, right, name, is_mutable);
}

void ModuleBuilder::CreateStore(const Value* ptr, const Value* value) {
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

const Value* ModuleBuilder::CreateAlloc(const Type* alloc_type, const std::string& name,
                                        bool is_mutable) {
    return CreateAlloc(alloc_type, CreateInt64Const(1), name, is_mutable);
}

const Value* ModuleBuilder::CreateAlloc(const Type* alloc_type, const Value* count,
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

const Value* ModuleBuilder::CreateAlloc(const Layout* layout, const std::string& name,
                                        bool is_mutable) {
    return CreateAlloc(layout, CreateInt64Const(1), name, is_mutable);
}

const Value* ModuleBuilder::CreateAlloc(const Layout* layout, const Value* count,
                                        const std::string& name, bool is_mutable) {
    const Variable* result = CurrentFunction()->AllocateVariable(
        Variable::Metadata(name, module_->Types()->GetPtr(), is_mutable));
    CurrentBlock()->Append(
        std::make_unique<AllocateLayout>(CurrentFunction(), layout, count, result));
    return result;
}

const Value* ModuleBuilder::CreateLoad(const Value* ptr, const Type* load_type,
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

void ModuleBuilder::CreateAssign(const Value* from, const Value* to) {
    check(to->IsMutable(), IRException("cannot assign to immutable",
                                       CurrentFunction(), CurrentBlock()));
    check(from->GetType() == to->GetType(),
          IRException("cannot assign from type " + from->GetType()->ToString() + " to " +
                             to->GetType()->ToString() + " without explicit cast",
                      CurrentFunction(), CurrentBlock()));
    current_block_->Append(std::make_unique<UnaryOperation>(
        CurrentFunction(), UnaryOperation::UnOp::ASSIGN, from, to));
}

const Value* ModuleBuilder::CreateAssign(const Value* from, const std::string& name,
                                         bool is_mutable) {
    const Variable* result =
        CurrentFunction()->AllocateVariable(Variable::Metadata(name, from->GetType(), is_mutable));
    current_block_->Append(std::make_unique<UnaryOperation>(
        CurrentFunction(), UnaryOperation::UnOp::ASSIGN, from, result));
    return result;
}

std::optional<const Value*> ModuleBuilder::CreateCall(const Function* func,
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

std::optional<const Value*> ModuleBuilder::CreateCall(const FunctionSignature* func_sig,
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

void ModuleBuilder::CreateReturnVoid() {
    check(!CurrentFunction()->GetSignature()->FuncType()->ReturnType().has_value(),
          IRException(CurrentFunction()->GetName() +
                             " has return type, but void is returned",
                      CurrentFunction(), CurrentBlock()));
    current_block_->Append(std::make_unique<ReturnVoidOp>(CurrentFunction()));
}

void ModuleBuilder::CreateReturnValue(const Value* value) {
    current_block_->Append(std::make_unique<ReturnValueOp>(CurrentFunction(), value));
}

const Value* ModuleBuilder::CreateGEP(const Value* ptr, const Layout* layout, int element_index,
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

const Value* ModuleBuilder::CastTo(const Value* value, const Type* target_type,
                                   const std::string& name, bool is_mutable) {
    const Variable* result = CreateVariable(name, target_type, is_mutable);
    current_block_->Append(std::make_unique<CastOperation>(CurrentFunction(), value, result));
    return result;
}

Function* ModuleBuilder::CurrentFunction() {
    assert(current_block_ != nullptr);
    return module_->GetFunction(current_block_->GetContextFunction()->GetName());
}

void ModuleBuilder::CreateBranch(const BasicBlock* target) {
    current_block_->Append(std::make_unique<BranchOperation>(CurrentFunction(), target));
    current_block_->TerminateBlock();
}

void ModuleBuilder::CreateConditionBranch(const Value* condtion, const BasicBlock* target_true,
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
const Value* ModuleBuilder::CreateCmp(const Value* left, const Value* right,
                                      const std::string& name, bool is_mutable) {
    const Variable* result = CreateVariable(name, module_->Types()->GetInt1(), is_mutable);
    current_block_->Append(
        std::make_unique<BinaryOperation>(CurrentFunction(), op, left, right, result));
    return result;
}

template <BinaryOperation::BinOp op>
const Value* ModuleBuilder::CreateArithmetic(const Value* left, const Value* right,
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
