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
#include <bier/core/types_registry.h>
#include <stdexcept>

using namespace bier;

namespace bier_tests {

TEST_CASE("Stable integer types", "[type_registry]") {
    DefaultTypesRegistry registry;
    REQUIRE(registry.GetInt1() == registry.GetInt1());
    REQUIRE(registry.GetInt8() == registry.GetInt8());
    REQUIRE(registry.GetInt16() == registry.GetInt16());
    REQUIRE(registry.GetInt8() != registry.GetInt32());
    REQUIRE(registry.GetInt32() == registry.GetInt32());
}

TEST_CASE("Same Function types", "[type_registry]") {
    DefaultTypesRegistry registry;
    const FunctionType* first = nullptr;
    const FunctionType* second = nullptr;
    {
        const Type* return_type = registry.GetInt32();
        std::vector<const Type*> args = {registry.GetPtr(), registry.GetInt32(),
                                         registry.GetInt8Ptr()};
        first = registry.MakeFunctionType(return_type, args);
        second = registry.MakeFunctionType(return_type, args);
        REQUIRE(first == second);
    }
    const FunctionType* different = nullptr;
    {
        const Type* return_type = registry.GetInt1();
        std::vector<const Type*> args = {registry.GetPtr(), registry.GetInt32(),
                                         registry.GetInt8Ptr()};
        different = registry.MakeFunctionType(return_type, args);
        REQUIRE(first != different);
    }
}

}  // namespace bier_tests
