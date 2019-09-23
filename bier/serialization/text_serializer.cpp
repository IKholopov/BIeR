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
#include "text_serializer.h"
#include "op_literals.h"
#include <bier/operations/ops.h>

namespace bier {

std::ostream& StringSerializer::TranslateFunctionSignature(const FunctionSignature* op,
                                                           std::ostream& stream) const {
    assert(op != nullptr);
    stream << "func " << op->Name() << " (";
    for (const auto arg : op->Arguments()) {
        stream << arg->GetType()->ToString() << " %" << arg->GetName() << ",";
    }
    stream << ") "
           << (op->ReturnType().has_value() ? op->ReturnType().value()->ToString() : "void");
    return stream;
}

std::ostream& StringSerializer::TranslateOp(const Operation* op, std::ostream& stream) const {
    auto return_val = op->GetReturnValue();
    if (return_val.has_value()) {
        stream << (return_val.value()->IsMutable() ? "$" : "%") << return_val.value()->GetName()
               << " " << return_val.value()->GetType()->ToString() << " = ";
    }
    stream << OpCodeName(op->OpCode()) << " ";
    if (op->OpCode() == OpCodes::ALLOC_LAYOUT_OP) {
        auto alloc_op = static_cast<const AllocateLayout*>(op);
        const Layout* layout = alloc_op->GetLayout();
        layout->Name().empty() ? TranslateLayout(layout, stream) : stream << "@" + layout->Name();
    }
    for (const Value* arg : op->GetArguments()) {
        TranslateValue(arg, stream);
    }
    if (op->OpCode() == OpCodes::BRANCH_OP || op->OpCode() == OpCodes::COND_BRANCH_OP) {
        auto branch_op = dynamic_cast<const Branch*>(op);
        for (auto block : branch_op->DestinationBlocks()) {
            stream << block->GetLabel() << ", ";
        }
    }
    return stream;
}

std::ostream& StringSerializer::TranslateValue(const Value* value, std::ostream& stream) const {
    stream << value->GetType()->ToString() << " ";
    const auto const_val = dynamic_cast<const ConstValue*>(value);
    if (const_val != nullptr) {
        stream << const_val->GetConstValue();
    } else {
        stream << (value->IsMutable() ? "$" : "%") << value->GetName();
        // TODO
    }
    stream << ", ";
    return stream;
}

std::ostream& StringSerializer::TranslateLayout(const Layout* layout, std::ostream& stream) const {
    stream << "[";
    for (const auto& entry : layout->Entries()) {
        stream << "[" << entry.type->ToString() << " x " << entry.count << "] ";
    }
    return stream << "]";
}

std::ostream& StringSerializer::PrintModule(const Module* module, std::ostream& stream) const {
    assert(module != nullptr);
    for (const auto& signature : module->GetExternalFunctions()) {
        TranslateFunctionSignature(signature, stream) << "\n";
    }
    for (const auto& [signature, body] : module->GetDefinedFunctions()) {
        TranslateFunctionSignature(signature, stream) << " {\n";
        for (const auto& block : body->GetBlocks()) {
            if (!block.GetLabel().empty()) {
                stream << block.GetLabel() << ":\n";
            }
            for (const auto& op : block.GetOperations()) {
                stream << "\t";
                TranslateOp(op.get(), stream) << "\n";
            }
        }
        stream << "}\n";
    }
    return stream;
}

std::string StringSerializer::OpCodeName(int code) const
{
    if (code >= Literal::StartingExtCode) {
        throw std::runtime_error("Name of opcode " + std::to_string(code) + " is not specified");
    }
    return Literal::OpCodeValue(code);
}

}  // namespace bier
