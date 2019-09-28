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
#include "module.h"

namespace bier {

Module::Module(std::unique_ptr<TypeRegistryInterface>&& typeRegistry)
    : types_(std::move(typeRegistry)) {
}

const Layout* Module::AddAnnonymousLayout(LayoutPtr&& layout) {
    const Layout* ptr = layout.get();
    anonymous_layouts_.emplace(std::move(layout));
    return ptr;
}

const Layout* Module::AddAnnonymousLayout(const std::vector<Layout::LayoutEntry>& entries) {
    auto layout = MakeLayout(entries);
    const Layout* ptr = layout.get();
    anonymous_layouts_.emplace(std::move(layout));
    return ptr;
}

const Layout* Module::AddNamedLayout(LayoutPtr&& layout, const std::string& name) {
    const Layout* ptr = layout.get();
    named_layouts_.insert({name, std::move(layout)});
    return ptr;
}

const Layout* Module::AddNamedLayout(const std::vector<Layout::LayoutEntry>& entries,
                                     const std::string& name) {
    auto layout = MakeLayout(entries);
    const Layout* ptr = layout.get();
    named_layouts_.insert({name, std::move(layout)});
    return ptr;
}

FunctionSignature* Module::AddExternalFunction(const std::string& name,
                                               const FunctionType* function_type) {
    FunctionSignature* signature = AddSignature(name, function_type);
    external_functions_.insert(signature);
    return signature;
}

Function* Module::AddFunction(const std::string& name, const FunctionType* function_type) {
    FunctionSignature* signature = AddSignature(name, function_type);
    auto function = std::make_unique<Function>(signature);
    Function* functionPtr = function.get();
    functions_.insert({signature, std::move(function)});
    return functionPtr;
}

bool Module::HasFunction(const std::string& name) const {
    return ContainerHas(function_sigs_, name);
}

Function* Module::GetFunction(const std::string& name) {
    check(ContainerHas(function_sigs_, name), IRException("unknown function " + name));
    const FunctionSignature* signature = function_sigs_.at(name).get();
    check(ContainerHas(functions_, signature), IRException("unknown function " + name));
    return functions_.at(signature).get();
}

const FunctionSignature* Module::GetFunctionSignature(const std::string& name) {
    check(ContainerHas(function_sigs_, name), IRException("unknown function " + name));
    return function_sigs_.at(name).get();
}

StaticData* Module::AddStaticData(const std::string& name, const Layout* layout) {
    check(!name.empty(), IRException("static data should be named"));
    auto data = std::make_unique<StaticData>(types_->DefaultTypes(), layout, name);
    StaticData* ptr = data.get();
    static_data_.insert({name, std::move(data)});
    return ptr;
}

const StaticData* Module::GetStaticData(const std::string& name) const {
    check(ContainerHas(static_data_, name), IRException("static data " + name
                                                        + " is not defined"));
    return static_data_.at(name).get();
}

FunctionSignature* Module::AddSignature(const std::string& name, const FunctionType* functionType) {
    check(types_->Has(functionType), IRException("Not registered in module"));
    check(!ContainerHas(function_sigs_, name),
          IRException(name + " already registered in the module"));
    FunctionSigPtr functionSignature = std::make_unique<FunctionSignature>(name, functionType);
    return function_sigs_.insert({name, std::move(functionSignature)}).first->second.get();
}

LayoutPtr Module::MakeLayout(const std::vector<Layout::LayoutEntry>& entries) const {
    auto layout = std::make_unique<Layout>();
    for (const auto& entry : entries) {
        layout->Add(entry);
    }

    return layout;
}

}  // namespace bier
