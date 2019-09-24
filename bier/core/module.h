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
#include <bier/core/exceptions.h>
#include <bier/core/function.h>
#include <bier/core/layout.h>
#include <bier/core/types_registry.h>
#include <bier/utils/iterator_range.h>

namespace bier {

class Module {
public:
    using TTypeRegister = DefaultTypesRegistry;

    explicit Module(std::unique_ptr<TypeRegistryInterface>&& typeRegistry =
                        std::make_unique<DefaultTypesRegistry>());

    // Types
    const DefaultTypesRegistry* Types() const {
        return types_->DefaultTypes();
    }
    DefaultTypesRegistry* Types() {
        return types_->DefaultTypes();
    }

    template <typename T>
    const T* ExtendedTypes() const {
        return dynamic_cast<const T*>(types_.get());
    }

    // Layouts
    const Layout* AddAnnonymousLayout(LayoutPtr&& layout);
    const Layout* AddAnnonymousLayout(const std::vector<Layout::LayoutEntry>& entries);
    const Layout* AddNamedLayout(LayoutPtr&& layout, const std::string& name);
    const Layout* AddNamedLayout(const std::vector<Layout::LayoutEntry>& entries,
                                 const std::string& name);
    const Layout* GetNamedLayout(const std::string& name) const {
        check(ContainerHas(named_layouts_, name),
              IRException("unknown layout \"" + name + "\""));
        return named_layouts_.at(name).get();
    }

    // Functions
    FunctionSignature* AddExternalFunction(const std::string& name,
                                           const FunctionType* function_type);
    Function* AddFunction(const std::string& name, const FunctionType* function_type);

    const FunctionSignature* GetFunctionSignature(const std::string& name) const {
        return function_sigs_.at(name).get();
    }

    auto GetDeclaredFunctions() const {
        return IteratorRange(function_sigs_);
    }

    auto GetDefinedFunctions() const {
        return IteratorRange(functions_);
    }

    auto GetExternalFunctions() const {
        return IteratorRange(external_functions_);
    }

    auto GetAnnonymousLayouts() const {
        return IteratorRange(anonymous_layouts_);
    }

    auto GetNamedLayouts() const {
        return IteratorRange(named_layouts_);
    }

    bool HasFunction(const std::string& name) const;
    Function* GetFunction(const std::string& name);
    const FunctionSignature* GetFunctionSignature(const std::string& name);

    bool IsExternalFunction(const FunctionSignature* signature) const {
        return ContainerHas(external_functions_, signature);
    }

private:
    std::unique_ptr<TypeRegistryInterface> types_;
    StdHashSet<LayoutPtr> anonymous_layouts_;
    StdHashMap<std::string, LayoutPtr> named_layouts_;
    StdHashMap<std::string, FunctionSigPtr> function_sigs_;
    HashPtrMap<FunctionSignature, FunctionPtr> functions_;
    StdHashSet<const FunctionSignature*> external_functions_;

    FunctionSignature* AddSignature(const std::string& name, const FunctionType* functionType);
    LayoutPtr MakeLayout(const std::vector<Layout::LayoutEntry>& entries) const;
};

using ModulePtr = std::unique_ptr<Module>;

}  // namespace bier
