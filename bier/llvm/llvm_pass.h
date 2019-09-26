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
#include <bier/pass/module_pass.h>
#include <llvm/IR/IRBuilder.h>

namespace bier {

class BuildLLVMIRPass : public ModulePass {
public:
    BuildLLVMIRPass(llvm::LLVMContext* context, llvm::Module* module);
    // ModulePass interface
    void Apply(ModulePtr&& module) override;

private:
    llvm::LLVMContext* context_ = nullptr;
    llvm::Module* llvm_ = nullptr;
    bier::Module* bier_module_ = nullptr;
    llvm::IRBuilder<> builder_;
    StdHashMap<const Value*, llvm::Value*> llvm_values_;
    StdHashMap<const BasicBlock*, llvm::BasicBlock*> llvm_blocks_;
    StdHashMap<const Layout*, llvm::StructType*> llvm_layouts_;
    const bier::Function* current_func_ = nullptr;
    const bier::BasicBlock* current_block_ = nullptr;

    llvm::FunctionType* ConvertFunctionType(const FunctionType* type);
    llvm::Type* ConvertBasicType(std::optional<const Type*> type);
    void CreateDeclaration(const FunctionSignature* signature, bool external);
    llvm::Function* CreateFunction(const Function* func);
    void TranslateOperation(const Operation* op);

    DefaultTypesRegistry* Types() const;

    template <typename FBuildOp>
    void TranslateBinOpSimple(const Operation* op, FBuildOp buildOp);
    template <typename FBuildOp>
    void TranslateBinOpArithmetic(const Operation* op, FBuildOp buildOp);
    template <typename FBuildOp>
    void TranslateBinOpRoundArithmetic(const Operation* op, FBuildOp buildOp);
    void TranslateGEP(const Operation* op);
    llvm::Value* LlvmValue(const Value* value);
    llvm::StructType* LlvmLayout(const Layout* value);

    llvm::Value* PtrCast(const Value* bier_from, const Type* bier_to_type);
};

}  // namespace bier
