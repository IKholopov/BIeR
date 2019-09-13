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
#include <bier/core/layout.h>
#include <bier/core/types_registry.h>

using namespace bier;

namespace bier_tests {

TEST_CASE("Correct on border", "[layout]") {
    DefaultTypesRegistry registry;
    Layout layout;
    layout.Add(registry.GetInt1());
    layout.AddArray(registry.GetInt8(), 10);
    layout.AddArray(registry.GetInt16(), 3);
    layout.AddArray(registry.GetPtr(), 2);

    REQUIRE(layout.GetEntry(0) == registry.GetInt1());
    REQUIRE(layout.GetEntry(1) == registry.GetInt8());
    REQUIRE(layout.GetEntry(11) == registry.GetInt16());
    REQUIRE(layout.GetEntry(14) == registry.GetPtr());
}

TEST_CASE("Correct between border", "[layout]") {
    DefaultTypesRegistry registry;
    Layout layout;
    layout.Add(registry.GetInt1());
    layout.AddArray(registry.GetInt8(), 10);
    layout.AddArray(registry.GetInt16(), 3);
    layout.AddArray(registry.GetPtr(), 2);

    REQUIRE(layout.GetEntry(2) == registry.GetInt8());
    REQUIRE(layout.GetEntry(10) == registry.GetInt8());
    REQUIRE(layout.GetEntry(13) == registry.GetInt16());
    REQUIRE(layout.GetEntry(15) == registry.GetPtr());
    REQUIRE_THROWS(layout.GetEntry(16));
}

}   // _bier_tests
