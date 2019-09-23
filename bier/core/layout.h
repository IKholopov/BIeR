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

#include <bier/core/type.h>
#include <bier/utils/iterator_range.h>
#include <vector>

namespace bier {

class Layout {
public:
    struct LayoutEntry {
        const Type* type = nullptr;
        int count = 1;
        /* int alignment = -1; */

        LayoutEntry() = default;

        LayoutEntry(const Type* _type) : type(_type) {
            assert(type != nullptr);
        }
        LayoutEntry(const Type* _type, int _count) : type(_type), count(_count) {
            assert(type != nullptr);
        }
    };

    int Add(const Type* type);
    int AddArray(const Type* type, int count);
    int Add(const LayoutEntry& entry);
    int Add(const Layout* layout);
    void SetName(const std::string& name) {
        name_ = name;
    }

    const Type* GetEntry(int index) const;
    auto Entries() const {
        return IteratorRange(entries_);
    }
    const std::string& Name() const {
        return name_;
    }
    int GetOffset(int idx, int element_offset) const {
        return offsets_[idx] + element_offset;
    }

private:
    std::vector<LayoutEntry> entries_;
    std::vector<int> offsets_;
    std::string name_;

    int GetNextOffset() const;
};

using LayoutPtr = std::unique_ptr<Layout>;

}  // namespace bier
