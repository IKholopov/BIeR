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
#include <bier/core/const_value.h>
#include <bier/core/module.h>
#include <bier/core/types_registry.h>
#include <ostream>
#include <vector>

namespace bier {

class ModuleBuilder {
public:
    explicit ModuleBuilder(Module* module_to_attach);

    template <typename F, F Method, typename ... Arg >
    auto DiagnosticCreate(Arg&& ... args) {
        try {
            return (this->*Method)(std::forward<Arg>(args)...);
        } catch (IRException& e) {
            e.SetPosition(position_);
            throw;
        }
    }

    Function* CreateFunction(const std::string& name,
                             std::optional<const Type*> return_type = std::nullopt,
                             const std::vector<const Type*>& arguments = {}) {
        return DiagnosticCreate<decltype(&ModuleBuilder::CreateFunctionImpl),
                &ModuleBuilder::CreateFunctionImpl>(name, return_type, arguments);
    }

    // Create new BasicBlock
    BasicBlock* CreateBlock(Function* function, const std::string& label = "",
                            BasicBlock* insert_after = nullptr) {
        return DiagnosticCreate<decltype(&ModuleBuilder::CreateBlockImpl),
                &ModuleBuilder::CreateBlockImpl>(function, label, insert_after);
    }

    // Create new Static Data
    const StaticData* CreateStaticData(const Layout* layout, std::vector<ValuePtr>&& values,
                                       const std::string& name) {
        return DiagnosticCreate<decltype(&ModuleBuilder::CreateStaticDataImpl),
                &ModuleBuilder::CreateStaticDataImpl>(layout, std::move(values), name);
    }

    // Create Instructions
    const Value* CreateInt1Const(bool value) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateInt1ConstImpl),
                &ModuleBuilder::CreateInt1ConstImpl>(value);
    }
    const Value* CreateInt8Const(uint64_t value) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateInt8ConstImpl),
                &ModuleBuilder::CreateInt8ConstImpl>(value);
    }
    const Value* CreateInt16Const(uint64_t value) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateInt16ConstImpl),
                &ModuleBuilder::CreateInt16ConstImpl>(value);
    }
    const Value* CreateInt32Const(uint64_t value) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateInt32ConstImpl),
                &ModuleBuilder::CreateInt32ConstImpl>(value);
    }
    const Value* CreateInt64Const(uint64_t value) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateInt64ConstImpl),
                &ModuleBuilder::CreateInt64ConstImpl>(value);
    }
    const Value* CreateNullPtr() {
        return CastTo(CreateInt64Const(0), module_->Types()->GetPtr());
    }

    const Value* CreateAdd(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAddImpl),
                &ModuleBuilder::CreateAddImpl>(left, right, name, is_mutable);
    }
    const Value* CreateSub(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateSubImpl),
                &ModuleBuilder::CreateSubImpl>(left, right, name, is_mutable);
    }
    const Value* CreateMul(const Value* left, const Value* right, const std::string& name = "",
                               bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateMulImpl),
                &ModuleBuilder::CreateMulImpl>(left, right, name, is_mutable);
    }
    const Value* CreateSDiv(const Value* left, const Value* right, const std::string& name = "",
                                bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateSDivImpl),
                &ModuleBuilder::CreateSDivImpl>(left, right, name, is_mutable);
    }
    const Value* CreateEQ(const Value* left, const Value* right, const std::string& name = "",
                              bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateEQImpl),
                &ModuleBuilder::CreateEQImpl>(left, right, name, is_mutable);
    }
    const Value* CreateNE(const Value* left, const Value* right, const std::string& name = "",
                              bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateNEImpl),
                &ModuleBuilder::CreateNEImpl>(left, right, name, is_mutable);
    }
    const Value* CreateSLE(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateSLEImpl),
                &ModuleBuilder::CreateSLEImpl>(left, right, name, is_mutable);
    }
    const Value* CreateSLT(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateSLTImpl),
                &ModuleBuilder::CreateSLTImpl>(left, right, name, is_mutable);
    }
    const Value* CreateSGE(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateSGEImpl),
                &ModuleBuilder::CreateSGEImpl>(left, right, name, is_mutable);
    }
    const Value* CreateSGT(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateSGTImpl),
                &ModuleBuilder::CreateSGTImpl>(left, right, name, is_mutable);
    }

    void CreateStore(const Value* ptr, const Value* value) {
        DiagnosticCreate<decltype (&ModuleBuilder::CreateStoreImpl),
                &ModuleBuilder::CreateStoreImpl>(ptr, value);
    }
    const Value* CreateAlloc(const Type* alloc_type, const std::string& name = "",
                                 bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAllocSingleImpl),
                &ModuleBuilder::CreateAllocSingleImpl>(alloc_type, name, is_mutable);
    }
    const Value* CreateAlloc(const Type* alloc_type, const Value* count, const std::string& name = "",
                             bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAllocArrayImpl),
                 &ModuleBuilder::CreateAllocArrayImpl>(alloc_type, count, name, is_mutable);
    }
    const Value* CreateAlloc(const Layout* layout, const std::string& name = "",
                             bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAllocLayoutSingleImpl),
                &ModuleBuilder::CreateAllocLayoutSingleImpl>(layout, name, is_mutable);
    }
    const Value* CreateAlloc(const Layout* layout, const Value* count, const std::string& name = "",
                             bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAllocLayoutArrayImpl),
                &ModuleBuilder::CreateAllocLayoutArrayImpl>(layout, count, name, is_mutable);
    }
    const Value* CreateLoad(const Value* ptr, const Type* load_type, const std::string& name = "",
                            bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateLoadImpl),
                &ModuleBuilder::CreateLoadImpl>(ptr, load_type, name, is_mutable);
    }
    void CreateAssign(const Value* from, const Value* to) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAssignBetweenImpl),
                &ModuleBuilder::CreateAssignBetweenImpl>(from, to);
    }
    const Value* CreateAssign(const Value* from, const std::string& name = "", bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateAssignImpl),
                &ModuleBuilder::CreateAssignImpl>(from, name, is_mutable);
    }
    std::optional<const Value*> CreateCall(const Function* func,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateCallFuncImpl),
                &ModuleBuilder::CreateCallFuncImpl>(func, args, name, is_mutable);
    }
    std::optional<const Value*> CreateCall(const FunctionSignature* func_sig,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateCallFuncSigImpl),
                &ModuleBuilder::CreateCallFuncSigImpl>(func_sig, args, name, is_mutable);
    }
    std::optional<const Value*> CreateCall(const FunctionType* func_type, const Value* func_ptr,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateCallFuncPtr), &ModuleBuilder::CreateCallFuncPtr>(
                    func_type, func_ptr, args, name, is_mutable);
    }

    void CreateReturnVoid() {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateReturnVoidImpl),
                &ModuleBuilder::CreateReturnVoidImpl>();
    }
    void CreateReturnValue(const Value* value) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateReturnValueImpl),
                &ModuleBuilder::CreateReturnValueImpl>(value);
    }

    const Value* CreateGEP(const Value* ptr, const Layout* layout, int element_index,
                           const std::string& name = "", bool is_mutable = false,
                           std::optional<const Value*> base_offset = std::nullopt,
                           std::optional<const Value*> element_offset = std::nullopt) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateGEPImpl),
                &ModuleBuilder::CreateGEPImpl>(ptr, layout, element_index, name, is_mutable,
                                                         base_offset, element_offset);
    }
    const Value* CastTo(const Value* value, const Type* target_type, const std::string& name = "",
                        bool is_mutable = false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CastToImpl),
                &ModuleBuilder::CastToImpl>(value, target_type, name, is_mutable);
    }


    void CreateBranch(const BasicBlock* target) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateBranchImpl),
                &ModuleBuilder::CreateBranchImpl>(target);
    }
    void CreateConditionBranch(const Value* condtion, const BasicBlock* target_true,
                                   const BasicBlock* target_false) {
        return DiagnosticCreate<decltype (&ModuleBuilder::CreateConditionBranchImpl),
                &ModuleBuilder::CreateConditionBranchImpl>(condtion, target_true, target_false);
    }    

    Function* CurrentFunction();
    BasicBlock* CurrentBlock() {
        return current_block_;
    }

    void AttachTo(BasicBlock* block);

    const SourcePos& GetPosition() const {
        return position_;
    }
    void SetPosition(const SourcePos& position) {
        position_ = position;
    }

private:
    SourcePos position_;
    Module* module_ = nullptr;
    BasicBlock* current_block_ = nullptr;

    template <typename TRegister, const Type* (TRegister::*FGetIntMethod)() const>
    const Value* CreateConstInt(uint64_t value, const TRegister* types);

    template <BinaryOperation::BinOp op>
    const Value* CreateCmp(const Value* left, const Value* right, const std::string& name,
                           bool is_mutable);
    template <BinaryOperation::BinOp op>
    const Value* CreateArithmetic(const Value* left, const Value* right, const std::string& name,
                                  bool is_mutable);

    const Variable* CreateVariable(const std::string& name, const Type* type, bool is_mutable);



    Function* CreateFunctionImpl(const std::string& name,
                             std::optional<const Type*> return_type = std::nullopt,
                             const std::vector<const Type*>& arguments = {});

    // Create new BasicBlock
    BasicBlock* CreateBlockImpl(Function* function, const std::string& label = "",
                            BasicBlock* insert_after = nullptr);

    const StaticData* CreateStaticDataImpl(const Layout* layout, std::vector<ValuePtr>&& values,
                                                      const std::string& name);

    // Create Instructions
    const Value* CreateInt1ConstImpl(bool value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt1>(
            value ? 1 : 0, module_->Types());
    }
    const Value* CreateInt8ConstImpl(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt8>(
            value, module_->Types());
    }
    const Value* CreateInt16ConstImpl(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt16>(
            value, module_->Types());
    }
    const Value* CreateInt32ConstImpl(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt32>(
            value, module_->Types());
    }
    const Value* CreateInt64ConstImpl(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt64>(
            value, module_->Types());
    }

    const Value* CreateAddImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSubImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateMulImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSDivImpl(const Value* left, const Value* right, const std::string& name = "",
                            bool is_mutable = false);
    const Value* CreateEQImpl(const Value* left, const Value* right, const std::string& name = "",
                          bool is_mutable = false);
    const Value* CreateNEImpl(const Value* left, const Value* right, const std::string& name = "",
                          bool is_mutable = false);
    const Value* CreateSLEImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSLTImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSGEImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSGTImpl(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);

    void CreateStoreImpl(const Value* ptr, const Value* value);
    const Value* CreateAllocSingleImpl(const Type* alloc_type, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateAllocArrayImpl(const Type* alloc_type, const Value* count, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateAllocLayoutSingleImpl(const Layout* layout, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateAllocLayoutArrayImpl(const Layout* layout, const Value* count, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateLoadImpl(const Value* ptr, const Type* load_type, const std::string& name = "",
                            bool is_mutable = false);
    void CreateAssignBetweenImpl(const Value* from, const Value* to);
    const Value* CreateAssignImpl(const Value* from, const std::string& name = "", bool is_mutable = false);
    std::optional<const Value*> CreateCallFuncImpl(const Function* func,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false);
    std::optional<const Value*> CreateCallFuncSigImpl(const FunctionSignature* func_sig,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false);
    std::optional<const Value*> CreateCallFuncPtr(const FunctionType* func_type, const Value* func_ptr,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false);

    void CreateReturnVoidImpl();
    void CreateReturnValueImpl(const Value* value);

    const Value* CreateGEPImpl(const Value* ptr, const Layout* layout, int element_index,
                           const std::string& name = "", bool is_mutable = false,
                           std::optional<const Value*> base_offset = std::nullopt,
                           std::optional<const Value*> element_offset = std::nullopt);
    const Value* CastToImpl(const Value* value, const Type* target_type, const std::string& name = "",
                        bool is_mutable = false);


    void CreateBranchImpl(const BasicBlock* target);
    void CreateConditionBranchImpl(const Value* condtion, const BasicBlock* target_true,
                                   const BasicBlock* target_false);
};

template <typename TRegister, const Type* (TRegister::*FGetIntMethod)() const>
const Value* ModuleBuilder::CreateConstInt(uint64_t value, const TRegister* types) {
    assert(current_block_ != nullptr);
    auto int_value = std::make_unique<IntegerConst>(
        value, static_cast<const IntTypeBase*>((types->*FGetIntMethod)()));
    const Value* return_val = int_value.get();
    current_block_->InsertConst(std::move(int_value));
    return return_val;
}



}  // namespace bier
