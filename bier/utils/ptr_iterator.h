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
#include <bier/utils/iterator_range.h>

namespace bier {

template <typename TContainer, typename TIterator=typename TContainer::const_iterator>
class PtrIterator {
public:
    using element_type = typename TContainer::value_type::element_type;

    PtrIterator(TIterator it) : iterator(it) {
    }

    PtrIterator operator++() {
        return ++iterator;
    }
    PtrIterator operator++(int) {
        return iterator++;
    }

    element_type* operator*() {
        return iterator->get();
    }
    const element_type* operator*() const {
        return iterator->get();
    }

    bool operator==(const PtrIterator& it) const {
        return iterator == it.iterator;
    }

    bool operator!=(const PtrIterator& it) const {
        return iterator != it.iterator;
    }

private:
    TIterator iterator;
};

template <typename TContainer>
using IteratorPtrRange = IteratorRange<TContainer, PtrIterator<TContainer>>;
template <typename TContainer>
using MutableIteratorPtrRange = IteratorRange<TContainer, PtrIterator<TContainer, typename TContainer::iterator>>;

}   // _bier
