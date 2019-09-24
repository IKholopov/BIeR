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
#include <bier/core/module.h>

using namespace bier;

namespace bier_tests {

TEST_CASE("Declare external function", "[functions_declaration]") {
    Module module;
    const FunctionType* i32__ptr_i32_i8ptr = nullptr;
    {
        const Type* return_type = module.Types()->GetInt32();
        std::vector<const Type*> args = {module.Types()->GetPtr(), module.Types()->GetInt32(),
                                         module.Types()->GetInt8Ptr()};
        i32__ptr_i32_i8ptr = module.Types()->MakeFunctionType(return_type, args);
    }
    const std::string name = "external_func";
    module.AddExternalFunction(name, i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDeclaredFunctions().Size() == 1);
    const auto& declared = module.GetDeclaredFunctions().begin()->second;
    REQUIRE(declared->Name() == name);
    REQUIRE(declared->FuncType() == i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDefinedFunctions().Size() == 0);
}

TEST_CASE("Declare internal function", "[functions_declaration]") {
    Module module;
    const FunctionType* i32__ptr_i32_i8ptr = nullptr;
    {
        const Type* return_type = module.Types()->GetInt32();
        std::vector<const Type*> args = {module.Types()->GetPtr(), module.Types()->GetInt32(),
                                         module.Types()->GetInt8Ptr()};
        i32__ptr_i32_i8ptr = module.Types()->MakeFunctionType(return_type, args);
    }
    const std::string name = "external_func";
    module.AddFunction(name, i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDeclaredFunctions().Size() == 1);
    const auto& declared = module.GetDeclaredFunctions().begin()->second;
    REQUIRE(declared->Name() == name);
    REQUIRE(declared->FuncType() == i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDefinedFunctions().Size() == 1);
    const auto& defined = module.GetDeclaredFunctions().begin()->second;
    REQUIRE(defined->Name() == name);
    REQUIRE(defined->FuncType() == i32__ptr_i32_i8ptr);
}

TEST_CASE("Cannot declare same name", "[functions_declaration]") {
    Module module;
    const FunctionType* i32__ptr_i32_i8ptr = nullptr;
    const FunctionType* i32__ = nullptr;
    {
        const Type* return_type = module.Types()->GetInt32();
        std::vector<const Type*> args = {module.Types()->GetPtr(), module.Types()->GetInt32(),
                                         module.Types()->GetInt8Ptr()};
        i32__ptr_i32_i8ptr = module.Types()->MakeFunctionType(return_type, args);
        i32__ = module.Types()->MakeFunctionType(return_type);
    }
    const std::string name = "external_func";
    module.AddExternalFunction(name, i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDeclaredFunctions().Size() == 1);
    REQUIRE_THROWS_AS(module.AddFunction(name, i32__), IRException);
}

TEST_CASE("Cannot declare with unknown type", "[functions_declaration]") {
    Module module;
    const FunctionType localVar;

    const std::string name = "external_func";
    REQUIRE_THROWS_AS(module.AddFunction(name, &localVar), IRException);
}

TEST_CASE("Can use same func type", "[functions_declaration]") {
    Module module;
    const FunctionType* i32__ptr_i32_i8ptr = nullptr;
    {
        const Type* return_type = module.Types()->GetInt32();
        std::vector<const Type*> args = {module.Types()->GetPtr(), module.Types()->GetInt32(),
                                         module.Types()->GetInt8Ptr()};
        i32__ptr_i32_i8ptr = module.Types()->MakeFunctionType(return_type, args);
    }
    const std::string name1 = "external_func";
    const std::string name2 = "other_func";
    module.AddExternalFunction(name1, i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDeclaredFunctions().Size() == 1);
    module.AddFunction(name2, i32__ptr_i32_i8ptr);
    REQUIRE(module.GetDeclaredFunctions().Size() == 2);
}

TEST_CASE("Retrive declared func", "[functions_declaration]") {
    Module module;
    const FunctionType* i32__ptr_i32_i8ptr = nullptr;
    {
        const Type* return_type = module.Types()->GetInt32();
        std::vector<const Type*> args = {module.Types()->GetPtr(), module.Types()->GetInt32(),
                                         module.Types()->GetInt8Ptr()};
        i32__ptr_i32_i8ptr = module.Types()->MakeFunctionType(return_type, args);
    }
    const std::string name = "external_func";
    Function* f = module.AddFunction(name, i32__ptr_i32_i8ptr);
    std::vector<std::string> names = {"ptr", "int32", "int8*"};
    std::size_t index = 0;
    for (auto arg : f->GetSignature()->Arguments()) {
        arg->SetName(names[index++]);
    }
    const Function* func;
    REQUIRE_NOTHROW(func = module.GetFunction(name));
    REQUIRE(func != nullptr);
    index = 0;
    for (const auto arg : func->GetSignature()->Arguments()) {
        REQUIRE(arg->GetName() == names[index++]);
    }
    REQUIRE_THROWS_AS(module.GetFunction("non-existen"), IRException);
}

}  // namespace bier_tests
