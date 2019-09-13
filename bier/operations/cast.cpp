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
#include "cast.h"

namespace bier {

CastOperation::CastOperation(const Function* funciton, const Value* from, const Value* to)
    : context_(funciton),
      from_(from),
      to_(to) {
    assert(from_ != nullptr);
    assert(to_ != nullptr);
    assert(context_ != nullptr);
}

const Function* CastOperation::GetContextFunction() const {
    return context_;
}

std::vector<const Value*> CastOperation::GetArguments() const {
    return { from_ };
}

void CastOperation::SubstituteArguments(const std::vector<const Value*>& args) {
    assert(args.size() == 1);
    from_ = args[0];
}

std::optional<const Value*> CastOperation::GetReturnValue() const {
    return to_;
}

void CastOperation::SubstituteReturnValue(const Value* return_value) {
    to_ = return_value;
}

}   // _bier
