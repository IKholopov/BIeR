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
#include "branch.h"
#include <bier/core/basic_block.h>
#include <bier/core/function.h>
#include <bier/core/exceptions.h>

namespace bier {

BranchOperation::BranchOperation(const Function* context, const BasicBlock* target)
    : context_(context), target_(target) {
    check(context_ == target_->GetContextFunction(),
          IRException("branch to block outside the function", context_));
    assert(!target_->GetLabel().empty());
}

std::vector<const Value*> BranchOperation::GetArguments() const {
    return {};
}

std::optional<const Variable*> BranchOperation::GetReturnValue() const {
    return std::nullopt;
}

std::vector<const BasicBlock*> BranchOperation::DestinationBlocks() const {
    return {target_};
}

ConditionalBranchOperation::ConditionalBranchOperation(const Function* context,
                                                       const Value* condition,
                                                       const BasicBlock* target_true,
                                                       const BasicBlock* target_false)
    : context_(context),
      target_true_(target_true),
      target_false_(target_false),
      condition_(condition) {
    check(context_ == target_true_->GetContextFunction() &&
              context_ == target_false_->GetContextFunction(),
          IRException("branch to block outside the function", context_));
    assert(!target_true_->GetLabel().empty());
    assert(!target_false_->GetLabel().empty());
}

std::vector<const Value*> ConditionalBranchOperation::GetArguments() const {
    return {condition_};
}

void ConditionalBranchOperation::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 1);
    condition_ = args[0];
}

std::optional<const Variable*> ConditionalBranchOperation::GetReturnValue() const {
    return std::nullopt;
}

std::vector<const BasicBlock*> ConditionalBranchOperation::DestinationBlocks() const {
    return {target_true_, target_false_};
}

}  // namespace bier
