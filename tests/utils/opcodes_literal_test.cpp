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
#include <catch2/catch.hpp>
#include <bier/serialization/op_literals.h>

using namespace bier;

namespace bier_tests {

TEST_CASE("Declare external function", "[functions_declaration]") {
    REQUIRE(Literal::OpCodeValue(OpCodes::ADD_OP) == "add");
    REQUIRE(Literal::OpCodeValue(OpCodes::EQ_OP) == "eq");
    REQUIRE(Literal::OpCodeValue(OpCodes::ALLOC_OP) == "alloc");
}

}   // _bier_tests
