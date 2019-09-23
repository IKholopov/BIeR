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
#include "layout.h"

namespace bier {

int Layout::Add(const Type* type) {
    const int offset = GetNextOffset();
    entries_.emplace_back(type);
    offsets_.emplace_back(offset);
    return offset;
}

int Layout::AddArray(const Type* type, int count) {
    const int offset = GetNextOffset();
    entries_.emplace_back(type, count);
    offsets_.emplace_back(offset);
    return offset;
}

int Layout::Add(const Layout::LayoutEntry& entry) {
    assert(entry.count > 0);
    const int offset = GetNextOffset();
    entries_.push_back(entry);
    offsets_.emplace_back(offset);
    return offset;
}

int Layout::Add(const Layout* layout) {
    assert(layout != nullptr);
    for (const LayoutEntry& entry : layout->entries_) {
        Add(entry);
    }

    return offsets_.back();
}

const Type* Layout::GetEntry(int index) const {
    assert(!offsets_.empty() && offsets_.size() == entries_.size());
    check(index >= 0 && index < GetNextOffset(),
          std::runtime_error("out of bound index " + std::to_string(index) + " requested"));
    auto element = std::lower_bound(offsets_.begin(), offsets_.end(), index);
    const bool between_entries = element == offsets_.end() || *element != index;
    ssize_t idx = element - offsets_.begin();
    if (between_entries) {
        idx -= 1;
    }
    assert(idx >= 0);
    return entries_[static_cast<std::size_t>(idx)].type;
}

int Layout::GetNextOffset() const {
    return offsets_.empty() ? 0 : offsets_.back() + entries_.back().count;
}

}  // namespace bier
