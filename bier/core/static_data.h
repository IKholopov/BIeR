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
#include <bier/core/layout.h>
#include <bier/core/types_registry.h>
#include <bier/core/value.h>

namespace bier {

class StaticData : public Value {
public:
    StaticData(DefaultTypesRegistry* types, const Layout* layout, const std::string& name);

    // Value interface
    const Type* GetType() const override;
    std::string GetName() const override;
    bool IsMutable() const override;

    const Layout* GetLayout() const {
        return layout_;
    }
    const Value* GetEntry(size_t index) const;
    void SetEntry(ValuePtr&& value, size_t index);

private:
    std::vector<ValuePtr> values_;
    const Layout* layout_ = nullptr;
    std::string name_;
    const Type* ptrType_ = nullptr;
};

using StaticDataPtr = std::unique_ptr<StaticData>;

}   // bier

