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
#include "llvm_pass.h"
#include <bier/operations/ops.h>
#include <llvm/IR/Verifier.h>

namespace bier {

BuildLLVMIRPass::BuildLLVMIRPass(llvm::LLVMContext* context, llvm::Module* module)
    : context_(context), llvm_(module), builder_(*context) {
}

void BuildLLVMIRPass::Apply(ModulePtr&& module) {
    bier_module_ = module.get();

    for (const auto& [name, signature] : module->GetDeclaredFunctions()) {
        CreateDeclaration(signature.get(), bier_module_->IsExternalFunction(signature.get()));
    }

    for (const auto& [name, func] : module->GetDefinedFunctions()) {
        auto function = CreateFunction(func.get());
        llvm::verifyFunction(*function, &llvm::errs());
    }

    llvm::verifyModule(*llvm_, &llvm::errs());
    bier_module_ = nullptr;
}

llvm::FunctionType* BuildLLVMIRPass::ConvertFunctionType(const FunctionType* type) {
    std::vector<llvm::Type*> args;
    for (const Type* arg : type->Arguments()) {
        args.push_back(ConvertBasicType(arg));
    }
    return llvm::FunctionType::get(ConvertBasicType(type->ReturnType()), args, false);
}

llvm::Type* BuildLLVMIRPass::ConvertBasicType(std::optional<const Type*> type) {
    if (!type.has_value()) {
        return llvm::Type::getVoidTy(*context_);
    }
    if (type == Types()->GetPtr()) {
        return llvm::Type::getInt8PtrTy(*context_);
    }
    if (type == Types()->GetInt1()) {
        return llvm::Type::getInt1Ty(*context_);
    }
    if (type == Types()->GetInt8()) {
        return llvm::Type::getInt8Ty(*context_);
    }
    if (type == Types()->GetInt16()) {
        return llvm::Type::getInt16Ty(*context_);
    }
    if (type == Types()->GetInt32()) {
        return llvm::Type::getInt32Ty(*context_);
    }
    if (type == Types()->GetInt64()) {
        return llvm::Type::getInt64Ty(*context_);
    }
    if (Types()->IsPtr(type.value())) {
        const TypedPtrType* typed_ptr = static_cast<const TypedPtrType*>(type.value());
        return ConvertBasicType(typed_ptr->GetUnderlying())->getPointerTo();
    }
    throw std::runtime_error("Failed to convert to llvm type: " + type.value()->ToString());
}

void BuildLLVMIRPass::CreateDeclaration(const FunctionSignature* signature, bool external) {
    llvm::FunctionType* func_type = ConvertFunctionType(signature->FuncType());
    external |= signature->GetName() == "main";
    llvm::Function* func = llvm::Function::Create(
        func_type, external ? llvm::Function::ExternalLinkage : llvm::Function::InternalLinkage,
        signature->Name(), llvm_);

    auto it = signature->Arguments().begin();
    for (auto& arg : func->args()) {
        assert(it != signature->Arguments().end());
        arg.setName((*it)->GetName());
        ++it;
    }
}

llvm::Function* BuildLLVMIRPass::CreateFunction(const Function* func) {
    llvm::Function* llvm_func = llvm_->getFunction(func->GetName());
    auto it = func->GetSignature()->Arguments().begin();
    for (auto& arg : llvm_func->args()) {
        assert(it != func->GetSignature()->Arguments().end());
        llvm_values_.insert({*it, &arg});
        arg.setName((*it)->GetName());
        ++it;
    }
    for (const auto& block : func->GetBlocks()) {
        llvm_blocks_.insert({&block, llvm::BasicBlock::Create(llvm_->getContext(),
                                                              block.GetLabel(), llvm_func)});
    }
    for (const auto& block : func->GetBlocks()) {
        llvm::BasicBlock* llvm_block = llvm_blocks_.at(&block);
        builder_.SetInsertPoint(llvm_block);
        for (const auto& op : block.GetOperations()) {
            TranslateOperation(op.get());
        }
    }
    llvm_values_.clear();
    return llvm_func;
}

void BuildLLVMIRPass::TranslateOperation(const Operation* op) {
    switch (op->OpCode()) {
        case OpCodes::Op::EQ_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateICmpEQ);
            break;
        case OpCodes::Op::GE_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateICmpSGE);
            break;
        case OpCodes::Op::GT_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateICmpSGT);
            break;
        case OpCodes::Op::LE_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateICmpSLE);
            break;
        case OpCodes::Op::LT_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateICmpSLT);
            break;
        case OpCodes::Op::NE_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateICmpNE);
            break;
        case OpCodes::Op::ADD_OP:
            TranslateBinOpArithmetic(op, &llvm::IRBuilder<>::CreateAdd);
            break;
        case OpCodes::Op::SUB_OP:
            TranslateBinOpArithmetic(op, &llvm::IRBuilder<>::CreateSub);
            break;
        case OpCodes::Op::MULT_OP:
            TranslateBinOpArithmetic(op, &llvm::IRBuilder<>::CreateMul);
            break;
        case OpCodes::Op::SDIV_OP:
            TranslateBinOpRoundArithmetic(op, &llvm::IRBuilder<>::CreateSDiv);
            break;
        case OpCodes::Op::SREM_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateSRem);
            break;
        case OpCodes::Op::UDIV_OP:
            TranslateBinOpRoundArithmetic(op, &llvm::IRBuilder<>::CreateUDiv);
            break;
        case OpCodes::Op::UREM_OP:
            TranslateBinOpSimple(op, &llvm::IRBuilder<>::CreateURem);
            break;
        case OpCodes::Op::STORE_OP: {
            const BinaryOperation* operation = static_cast<const BinaryOperation*>(op);
            llvm::Value* ptr = PtrCast(operation->RightValue(),
                                       Types()->GetPtrTo(operation->LeftValue()->GetType()));
            builder_.CreateStore(LlvmValue(operation->LeftValue()),
                                 ptr);
        } break;
        case OpCodes::Op::ALLOC_OP: {
            const Value* return_val = op->GetReturnValue().value();
            const Type* underlying =
                static_cast<const TypedPtrType*>(op->GetReturnValue().value()->GetType())
                    ->GetUnderlying();
            llvm::Value* llvm_return = builder_.CreateAlloca(
                ConvertBasicType(underlying), LlvmValue(op->GetArguments().front()),
                op->GetReturnValue().value()->GetName());
            llvm_values_.insert({return_val, llvm_return});
        } break;
        case OpCodes::Op::LOAD_OP: {
            const Value* return_val = op->GetReturnValue().value();
            llvm::Value* src_val = PtrCast(op->GetArguments().front(), Types()->GetPtrTo(return_val->GetType()));
            llvm::Value* llvm_return = builder_.CreateLoad(src_val);
            llvm_values_.insert({return_val, llvm_return});
        } break;
        case OpCodes::Op::ASSIGN_OP: {
            const Value* return_val = op->GetReturnValue().value();
            llvm_values_.insert({return_val, LlvmValue(op->GetArguments().front())});
        } break;
        case OpCodes::Op::CONST_OP: {
            llvm_values_.insert(
                {op->GetReturnValue().value(), LlvmValue(op->GetArguments().front())});
        } break;
        case OpCodes::Op::RETVOID_OP:
            builder_.CreateRetVoid();
            break;
        case OpCodes::Op::RETVALUE_OP:
            builder_.CreateRet(LlvmValue(op->GetArguments().front()));
            break;
        case OpCodes::Op::GEP_OP:
            TranslateGEP(op);
            break;
        case OpCodes::Op::CALL_OP: {
            auto operation = static_cast<const CallOp*>(op);
            const Value* callee = operation->Callee();
            auto arguments = operation->GetArguments();
            std::vector<llvm::Value*> args;
            auto args_signature = operation->FuncType()->Arguments();
            for (std::size_t i = 1; i < arguments.size(); ++i) {
                const Type* arg_type = args_signature[i - 1];
                const Value* arg = arguments[i];
                args.push_back(PtrCast(arg, arg_type));
            }
            const std::string return_name =
                op->GetReturnValue().has_value() ? op->GetReturnValue().value()->GetName() : "";
            llvm::Value* llvm_ret = nullptr;
            auto callee_func_sig = dynamic_cast<const FunctionSignature*>(callee);
            auto callee_func = dynamic_cast<const Function*>(callee);
            if (callee_func_sig != nullptr) {
                llvm_ret = builder_.CreateCall(llvm_->getFunction(callee_func_sig->GetName()), args,
                                               return_name);
            } else if (callee_func != nullptr) {
                llvm_ret = builder_.CreateCall(llvm_->getFunction(callee_func->GetName()), args,
                                               return_name);
            } else {
                llvm_ret = builder_.CreateCall(ConvertFunctionType(operation->FuncType()),
                                               LlvmValue(callee), args, return_name);
            }

            if (op->GetReturnValue().has_value()) {
                llvm_values_.insert({op->GetReturnValue().value(), llvm_ret});
            }
        } break;
        case OpCodes::Op::COND_BRANCH_OP: {
            auto operation = static_cast<const ConditionalBranchOperation*>(op);
            builder_.CreateCondBr(LlvmValue(operation->GetArguments().front()),
                                  llvm_blocks_.at(operation->DestinationBlocks()[0]),
                                  llvm_blocks_.at(operation->DestinationBlocks()[1]));
        } break;
        case OpCodes::Op::BRANCH_OP: {
            auto operation = static_cast<const BranchOperation*>(op);
            builder_.CreateBr(llvm_blocks_.at(operation->DestinationBlocks().front()));
        } break;
        case OpCodes::Op::CAST_OP: {
            const Value* src = op->GetArguments().front();
            const Value* target = op->GetReturnValue().value();
            const Type* src_type = src->GetType();
            const Type* target_type = target->GetType();
            llvm::CastInst::CastOps cast_op = llvm::CastInst::CastOps::BitCast;
            llvm::Value* return_value = nullptr;
            if (Types()->IsPtr(src_type) && Types()->IsInteger(target_type)) {
                cast_op = llvm::CastInst::CastOps::PtrToInt;
            }
            else if (Types()->IsInteger(src_type) && Types()->IsPtr(target_type)) {
                cast_op = llvm::CastInst::CastOps::IntToPtr;
            } else if (Types()->IsInteger(src_type) && Types()->IsInteger(target_type)) {
                return_value = builder_.CreateIntCast(LlvmValue(src),
                                                        ConvertBasicType(target_type), true,
                                                        op->GetReturnValue().value()->GetName());
                llvm_values_.insert({target, return_value});
                break;
            }
            return_value = builder_.CreateCast(cast_op, LlvmValue(src),
                                                               ConvertBasicType(target_type),
                                                               op->GetReturnValue().value()->GetName());
            llvm_values_.insert({target, return_value});
        } break;
        case OpCodes::Op::ALLOC_LAYOUT_OP: {
            auto operation = static_cast<const AllocateLayout*>(op);
            const Value* count = operation->GetArguments().front();
            llvm_values_.insert({op->GetReturnValue().value(),
                                builder_.CreateAlloca(LlvmLayout(operation->GetLayout()), LlvmValue(count))});
        } break;
        default:
            throw std::runtime_error("not supported opcode: " + std::to_string(op->OpCode()));
            // TODO
    }
}

DefaultTypesRegistry* BuildLLVMIRPass::Types() const {
    return bier_module_->Types();
}

void BuildLLVMIRPass::TranslateGEP(const Operation* op) {
    auto operation = static_cast<const GEPOp*>(op);
    llvm::Value* base_offset = operation->BaseOffset().has_value()
                                   ? LlvmValue(operation->BaseOffset().value())
                                   : builder_.getInt32(0);
    llvm::Value* element_offset = operation->ElementOffset().has_value()
                                      ? LlvmValue(operation->ElementOffset().value())
                                      : builder_.getInt32(0);
    const Layout* layout = operation->GetLayout();
    llvm::StructType* struct_type = LlvmLayout(layout);
    llvm::Value* src_ptr = LlvmValue(op->GetArguments().front());
    src_ptr = builder_.CreateCast(llvm::Instruction::CastOps::BitCast, src_ptr, struct_type->getPointerTo());
    assert(!struct_type->isPointerTy());
    std::vector<llvm::Value*> offsets{base_offset, builder_.getInt32(operation->ElementIndex())};
    if (operation->ElementOffset().has_value()) {
        offsets.push_back(element_offset);
    }
    llvm::Value* return_val = builder_.CreateGEP(
        struct_type, src_ptr,
        offsets,
        operation->GetReturnValue().value()->GetName());
    llvm_values_.insert({op->GetReturnValue().value(), return_val});
}

llvm::Value* BuildLLVMIRPass::LlvmValue(const Value* value) {
    auto int_val = dynamic_cast<const IntegerConst*>(value);
    if (int_val != nullptr) {
        return builder_.getIntN(int_val->IntType()->GetNBits(), int_val->GetValue());
    }
    return llvm_values_.at(value);
}

llvm::StructType* BuildLLVMIRPass::LlvmLayout(const Layout* value) {
    if (ContainerHas(llvm_layouts_, value)) {
        return llvm_layouts_.at(value);
    }
    std::vector<llvm::Type*> struct_contents;
    for (const auto& member : value->Entries()) {
        for (int i = 0; i < member.count; ++i) {
            struct_contents.emplace_back(ConvertBasicType(member.type));
        }
    }
    auto llvm_struct = llvm::StructType::get(*context_, struct_contents);
    llvm_layouts_.insert({value, llvm_struct});
    return llvm_struct;
}

llvm::Value* BuildLLVMIRPass::PtrCast(const Value* bier_from, const Type* bier_to_type) {
    auto from = LlvmValue(bier_from);
    auto to_type = ConvertBasicType(bier_to_type);
    if ((!from->getType()->isPointerTy() && !to_type->isPointerTy())
            || (from->getType() == to_type)) {
        return from;
    }

    if (to_type == builder_.getInt8PtrTy()) {
        return builder_.CreateCast(llvm::Instruction::CastOps::BitCast, from,
                                   builder_.getInt8PtrTy());
    }
    return builder_.CreateCast(llvm::Instruction::CastOps::BitCast, from,
                               to_type);
}

template <typename FBuildOp>
void BuildLLVMIRPass::TranslateBinOpSimple(const Operation* op, FBuildOp buildOp) {
    const BinaryOperation* operation = static_cast<const BinaryOperation*>(op);
    llvm::Value* return_val =
        (builder_.*buildOp)(LlvmValue(operation->LeftValue()), LlvmValue(operation->RightValue()),
                            operation->GetReturnValue().value()->GetName());
    llvm_values_.insert({operation->GetReturnValue().value(), return_val});
}

template <typename FBuildOp>
void BuildLLVMIRPass::TranslateBinOpArithmetic(const Operation* op, FBuildOp buildOp) {
    const BinaryOperation* operation = static_cast<const BinaryOperation*>(op);
    llvm::Value* return_val =
        (builder_.*buildOp)(LlvmValue(operation->LeftValue()), LlvmValue(operation->RightValue()),
                            operation->GetReturnValue().value()->GetName(), false, false);
    llvm_values_.insert({operation->GetReturnValue().value(), return_val});
}

template <typename FBuildOp>
void BuildLLVMIRPass::TranslateBinOpRoundArithmetic(const Operation* op, FBuildOp buildOp) {
    const BinaryOperation* operation = static_cast<const BinaryOperation*>(op);
    llvm::Value* return_val =
        (builder_.*buildOp)(LlvmValue(operation->LeftValue()), LlvmValue(operation->RightValue()),
                            operation->GetReturnValue().value()->GetName(), false);
    llvm_values_.insert({operation->GetReturnValue().value(), return_val});
}

}  // namespace bier
