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
#include "op_literals.h"
#include <bier/operations/opcodes.h>

namespace bier {

// BinOp
template <>
struct OpLiteral<OpCodes::ADD_OP> {
    static constexpr const char* Literal = "add";
};
template <>
struct OpLiteral<OpCodes::SUB_OP> {
    static constexpr const char* Literal = "sub";
};
template <>
struct OpLiteral<OpCodes::MULT_OP> {
    static constexpr const char* Literal = "mul";
};
template <>
struct OpLiteral<OpCodes::UDIV_OP> {
    static constexpr const char* Literal = "udiv";
};
template <>
struct OpLiteral<OpCodes::SDIV_OP> {
    static constexpr const char* Literal = "sdiv";
};
template <>
struct OpLiteral<OpCodes::UREM_OP> {
    static constexpr const char* Literal = "urem";
};
template <>
struct OpLiteral<OpCodes::SREM_OP> {
    static constexpr const char* Literal = "srem";
};
template <>
struct OpLiteral<OpCodes::EQ_OP> {
    static constexpr const char* Literal = "eq";
};
template <>
struct OpLiteral<OpCodes::NE_OP> {
    static constexpr const char* Literal = "ne";
};
template <>
struct OpLiteral<OpCodes::LE_OP> {
    static constexpr const char* Literal = "le";
};
template <>
struct OpLiteral<OpCodes::LT_OP> {
    static constexpr const char* Literal = "lt";
};
template <>
struct OpLiteral<OpCodes::GE_OP> {
    static constexpr const char* Literal = "ge";
};
template <>
struct OpLiteral<OpCodes::GT_OP> {
    static constexpr const char* Literal = "gt";
};
template <>
struct OpLiteral<OpCodes::STORE_OP> {
    static constexpr const char* Literal = "store";
};

// UnOp
template <>
struct OpLiteral<OpCodes::ALLOC_OP> {
    static constexpr const char* Literal = "alloc";
};
template <>
struct OpLiteral<OpCodes::LOAD_OP> {
    static constexpr const char* Literal = "load";
};

template <>
struct OpLiteral<OpCodes::ASSIGN_OP> {
    static constexpr const char* Literal = "assign";
};

// Const
template <>
struct OpLiteral<OpCodes::CONST_OP> {
    static constexpr const char* Literal = "const";
};

// Return
template <>
struct OpLiteral<OpCodes::RETVOID_OP> {
    static constexpr const char* Literal = "ret";
};
template <>
struct OpLiteral<OpCodes::RETVALUE_OP> {
    static constexpr const char* Literal = "retval";
};

// GEP
template <>
struct OpLiteral<OpCodes::GEP_OP> {
    static constexpr const char* Literal = "gep";
};

// CALL
template <>
struct OpLiteral<OpCodes::CALL_OP> {
    static constexpr const char* Literal = "call";
};

// BRANCH
template <>
struct OpLiteral<OpCodes::BRANCH_OP> {
    static constexpr const char* Literal = "branch";
};

template <>
struct OpLiteral<OpCodes::COND_BRANCH_OP> {
    static constexpr const char* Literal = "cond";
};

// CAST
template <>
struct OpLiteral<OpCodes::CAST_OP> {
    static constexpr const char* Literal = "cast";
};

// Alloc layout
template <>
struct OpLiteral<OpCodes::ALLOC_LAYOUT_OP> {
    static constexpr const char* Literal = "alloc_layout";
};

LiteralArray<0>::LiteralArray() : Value(OpLiteral<0>::Literal){};

const Literal Literal::instance_;

std::string Literal::OpCodeValue(int op_code) {
    assert(op_code < StartingExtCode);
    return instance_.Get(op_code);
}

const int Literal::StartingExtCode = static_cast<int>(OpCodes::OPS_COUNT);

const char* Literal::Get(int op_code) const {
    static_assert(
        sizeof(LiteralArray<OpCodes::OPS_COUNT - 1>) == OpCodes::OPS_COUNT * sizeof(void*),
        "sizes do not match");
    static_assert(alignof(LiteralArray<OpCodes::OPS_COUNT - 1>) == sizeof(void*),
                  "alignment do not match");

    assert(op_code >= 0);
    assert(op_code < OpCodes::OPS_COUNT);
    return *((&(LiteralArray<0>::Value)) + op_code);
}

}  // namespace bier
