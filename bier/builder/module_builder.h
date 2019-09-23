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

    Function* CreateFunction(const std::string& name,
                             std::optional<const Type*> return_type = std::nullopt,
                             const std::vector<const Type*>& arguments = {});

    // Create new BasicBlock
    BasicBlock* CreateBlock(Function* function, const std::string& label = "",
                            BasicBlock* insert_after = nullptr);

    // Create Instructions
    const Value* CreateInt1Const(bool value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt1>(
            value ? 1 : 0, module_->Types());
    }
    const Value* CreateInt8Const(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt8>(
            value, module_->Types());
    }
    const Value* CreateInt16Const(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt16>(
            value, module_->Types());
    }
    const Value* CreateInt32Const(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt32>(
            value, module_->Types());
    }
    const Value* CreateInt64Const(uint64_t value) {
        return CreateConstInt<Module::TTypeRegister, &Module::TTypeRegister::GetInt64>(
            value, module_->Types());
    }
    const Value* CreateNullPtr() {
        return CastTo(CreateInt64Const(0), module_->Types()->GetPtr());
    }

    const Value* CreateAdd(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSub(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateMul(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSDiv(const Value* left, const Value* right, const std::string& name = "",
                            bool is_mutable = false);
    const Value* CreateEQ(const Value* left, const Value* right, const std::string& name = "",
                          bool is_mutable = false);
    const Value* CreateNE(const Value* left, const Value* right, const std::string& name = "",
                          bool is_mutable = false);
    const Value* CreateSLE(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSLT(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSGE(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);
    const Value* CreateSGT(const Value* left, const Value* right, const std::string& name = "",
                           bool is_mutable = false);

    void CreateStore(const Value* ptr, const Value* value);
    const Value* CreateAlloc(const Type* alloc_type, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateAlloc(const Type* alloc_type, const Value* count, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateAlloc(const Layout* layout, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateAlloc(const Layout* layout, const Value* count, const std::string& name = "",
                             bool is_mutable = false);
    const Value* CreateLoad(const Value* ptr, const Type* load_type, const std::string& name = "",
                            bool is_mutable = false);
    void CreateAssign(const Value* from, const Value* to);
    const Value* CreateAssign(const Value* from, const std::string& name = "", bool is_mutable = false);
    std::optional<const Value*> CreateCall(const Function* func,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false);
    std::optional<const Value*> CreateCall(const FunctionSignature* func_sig,
                                           const std::vector<const Value*>& args = {},
                                           const std::string& name = "", bool is_mutable = false);

    void CreateReturnVoid();
    void CreateReturnValue(const Value* value);

    const Value* CreateGEP(const Value* ptr, const Layout* layout, int element_index,
                           const std::string& name = "", bool is_mutable = false,
                           std::optional<const Value*> base_offset = std::nullopt,
                           std::optional<const Value*> element_offset = std::nullopt);
    const Value* CastTo(const Value* value, const Type* target_type, const std::string& name = "",
                        bool is_mutable = false);

    Function* CurrentFunction();
    BasicBlock* CurrentBlock() {
        return current_block_;
    }

    void CreateBranch(const BasicBlock* target);
    void CreateConditionBranch(const Value* condtion, const BasicBlock* target_true,
                               const BasicBlock* target_false);

    void AttachTo(BasicBlock* block);

private:
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
