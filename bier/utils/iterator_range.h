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
#include <utility>

namespace bier {

template<typename TContainer, typename TIterator=typename TContainer::const_iterator>
class OneWayIteratorRange {
public:
    using iterator = TIterator;

    explicit OneWayIteratorRange(const TContainer& container)
        : begin_(container.begin()), end_(container.end()) {
    }
    explicit OneWayIteratorRange(TContainer& container)
        : begin_(container.begin()), end_(container.end()) {

    }
    explicit OneWayIteratorRange(const TContainer* container)
        : begin_(container->begin()), end_(container->end()) {
    }
    explicit OneWayIteratorRange(TContainer* container)
        : begin_(container->begin()), end_(container->end()) {

    }

    iterator begin() const {
        return begin_;
    }
    iterator end() const {
        return end_;
    }

private:
    iterator begin_;
    iterator end_;
};

template<typename TContainer, typename TIterator=typename TContainer::const_iterator>
class BaseIteratorRange {
public:
    using iterator = TIterator;

    BaseIteratorRange(TIterator from, TIterator to, std::size_t size)
        : begin_(from), end_(to), size_(size) {
    }

    iterator begin() const {
        return begin_;
    }
    iterator end() const {
        return end_;
    }
    std::size_t Size() const {
        return size_;
    }

private:
    iterator begin_;
    iterator end_;
    std::size_t size_;
};

template<typename TContainer, typename TIterator=typename TContainer::const_iterator>
class IteratorRange : public BaseIteratorRange<TContainer, TIterator> {
public:
    using iterator = TIterator;

    explicit IteratorRange(const TContainer& container)
        : BaseIteratorRange<TContainer, TIterator>(container.begin(), container.end(), container.size()) {
    }
    explicit IteratorRange(TContainer& container)
        : BaseIteratorRange<TContainer, TIterator>(container.begin(), container.end(), container.size()) {

    }
    explicit IteratorRange(const TContainer* container)
        : BaseIteratorRange<TContainer, TIterator>(container->begin(), container->end(), container->size()) {
    }
    explicit IteratorRange(TContainer* container)
        : BaseIteratorRange<TContainer, TIterator>(container->begin(), container->end(), container->size()) {

    }
};

template<typename TContainer, typename TIterator=typename TContainer::const_iterator>
class InversedIteratorRange : public BaseIteratorRange<TContainer, TIterator> {
public:
    using iterator = TIterator;

    explicit InversedIteratorRange(const TContainer& container)
        : BaseIteratorRange<TContainer, TIterator>(container.rbegin(), container.rend(), container.size()) {
    }
    explicit InversedIteratorRange(TContainer& container)
        : BaseIteratorRange<TContainer, TIterator>(container.rbegin(), container.rend(), container.size()) {

    }
    explicit InversedIteratorRange(const TContainer* container)
        : BaseIteratorRange<TContainer, TIterator>(container->rbegin(), container->rend(), container->size()) {
    }
    explicit InversedIteratorRange(TContainer* container)
        : BaseIteratorRange<TContainer, TIterator>(container->rbegin(), container->rend(), container->size()) {
    }
};

template <typename TContainer>
using MutableIteratorRange = IteratorRange<TContainer, typename TContainer::iterator>;

}   // _bier
