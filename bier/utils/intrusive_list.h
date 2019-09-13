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
#include <memory>

namespace bier {

template <typename T>
class IntrusiveListNode {
public:
    using Ptr = std::unique_ptr<T>;
    class Iterator {
    public:
        explicit Iterator(IntrusiveListNode* node) : node_(node) {}

        const T& operator*() const {
            return static_cast<const T&>(*node_);
        }
        T& operator*() {
            return static_cast<T&>(*node_);
        }
        const T& operator->() const {
            return static_cast<const T&>(*node_);
        }
        T& operator->() {
            return static_cast<T&>(*node_);
        }
        bool operator==(const Iterator& other) const {
            return node_ == other.node_;
        }
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
        Iterator& operator++() {
            node_ = node_->Next();
            return *this;
        }

    private:
        IntrusiveListNode* node_;
    };

    using iterator = Iterator;
    using const_iterator = const Iterator;

    auto begin() {
        return IntrusiveListNode::Iterator(this);
    }
    auto end() {
        return IntrusiveListNode::Iterator(nullptr);
    }

    auto begin() const {
        return IntrusiveListNode::Iterator(this);
    }
    auto end() const {
        return IntrusiveListNode::Iterator(nullptr);
    }

    void InsertNext(Ptr&& toInsert) {
        toInsert->next_ = std::move(next_);
        next_ = std::move(toInsert);
    }
    void Attach(Ptr&& toInsert) {
        next_ = std::move(toInsert);
    }
    Ptr DetachNext() {
        Ptr toDetach = std::move(next_);
        next_ = std::move(toDetach->next_);

        return toDetach;
    }

    const T* Next() const {
        return next_.get();
    }
    T* Next() {
        return next_.get();
    }

private:
    Ptr next_;
};

}   // _bier
