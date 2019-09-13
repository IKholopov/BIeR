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

#include <bier/core/layout.h>
#include <bier/core/operation.h>
#include <bier/operations/opcodes.h>

namespace bier {

class GEPOp : public BaseOperation<OpCodes::Op::GEP_OP> {
public:
    GEPOp(const Function* func, const Value* ptr, int element_index,
          const Value* return_value, const Layout* layout,
          std::optional<const Value*> base_offset = std::nullopt,
          std::optional<const Value*> element_offset = std::nullopt);

    // FunctionContextMemeber interface
    const Function* GetContextFunction() const override;

    // Operation interface
    std::vector<const Value*> GetArguments() const override;
    void SubstituteArguments(const std::vector<const Value*>& args) override;
    std::optional<const Value*> GetReturnValue() const override;
    void SubstituteReturnValue(const Value* return_value) override;

    const Layout* GetLayout() const {
        return mem_layout_;
    }

    int ElementIndex() const {
        return index_;
    }
    std::optional<const Value*> BaseOffset() const {
        return base_offset_;
    }
    std::optional<const Value*> ElementOffset() const {
        return element_offset_;
    }

private:
    const Function* context_ = nullptr;
    const Value* ptr_ = nullptr;
    int index_ = -1;
    const Value* return_ptr_ = nullptr;
    const Layout* mem_layout_ = nullptr;
    std::optional<const Value*> base_offset_;
    std::optional<const Value*> element_offset_;
};

}   // _bier
