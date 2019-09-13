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
#include "gep.h"
#include <bier/core/function.h>

namespace bier {

GEPOp::GEPOp(const Function* func, const Value* ptr, int element_index,
             const Value* return_value, const Layout* layout,
             std::optional<const Value*> base_offset,
             std::optional<const Value*> element_offset)
    : context_(func),
      ptr_(ptr),
      index_(element_index),
      return_ptr_(return_value),
      mem_layout_(layout),
      base_offset_(base_offset),
      element_offset_(element_offset) {
    auto typed_ptr = dynamic_cast<const TypedPtrType*>(return_ptr_->GetType());
    check((typed_ptr != nullptr && typed_ptr->GetUnderlying() == mem_layout_->GetEntry(index_))
          || dynamic_cast<const PtrType*>(return_ptr_->GetType()) != nullptr,
          std::runtime_error("cannot assign pointer of " + mem_layout_->GetEntry(element_index)->ToString()
                             + " to " + return_ptr_->GetType()->ToString()));
}

const Function* GEPOp::GetContextFunction() const {
    return context_;
}

std::vector<const Value*> GEPOp::GetArguments() const {
    std::vector<const Value*> arguments = { ptr_ };
    if (base_offset_.has_value()) {
        arguments.push_back(base_offset_.value());
    }
    if (element_offset_.has_value()) {
        arguments.push_back(element_offset_.value());
    }
    return arguments;
}

void GEPOp::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(GetArguments().size() == args.size());
    ptr_ = args[0];
    std::size_t index = 1;
    if (base_offset_.has_value()) {
        base_offset_ = args[index++];
    }
    if (element_offset_.has_value()) {
        element_offset_ = args[index++];
    }
}

std::optional<const Value*> GEPOp::GetReturnValue() const {
    return return_ptr_;
}

void GEPOp::SubstituteReturnValue(const Value* return_value) {
    return_ptr_ = return_value;
}



}   // _bier
