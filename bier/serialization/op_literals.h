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
#include <bier/common.h>
#include <bier/operations/opcodes.h>

namespace bier {

template <int IOpCode>
struct OpLiteral {};

template <int IOpCode>
struct LiteralArray : public LiteralArray<IOpCode - 1> {
    const char* const Value = OpLiteral<IOpCode>::Literal;
};

template <>
struct LiteralArray<0> {
    const char* const Value;
    LiteralArray<0>();
};

class Literal : private LiteralArray<OpCodes::OPS_COUNT - 1> {
public:
    static std::string OpCodeValue(int op_code);
    static const int StartingExtCode;

private:
    static const Literal instance_;
    const char* Get(int op_code) const;
};

}  // namespace bier
