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
#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace bier {

class BasicBlock;
class FunctionSignature;
class Function;
class Operation;
class Value;
class Type;

using BasicBlockPtr = std::unique_ptr<BasicBlock>;
using FunctionSigPtr = std::unique_ptr<FunctionSignature>;
using FunctionPtr = std::unique_ptr<Function>;
using ValuePtr = std::unique_ptr<Value>;
using OperationPtr = std::unique_ptr<Operation>;
using TypePtr = std::unique_ptr<Type>;

using HashType = std::size_t;

template <typename T>
using HashPtrSet = std::unordered_set<const T*, typename T::HashPtr, typename T::PredPtr>;
template <typename TKey, typename TValue>
using HashPtrMap =
    std::unordered_map<const TKey*, TValue, typename TKey::HashPtr, typename TKey::PredPtr>;

template <typename T>
using HashSet = std::unordered_set<T, typename T::Hash>;
template <typename TKey, typename TValue>
using HashMap = std::unordered_map<TKey, TValue, typename TKey::Hash>;

template <typename T>
using StdHashSet = std::unordered_set<T>;
template <typename TKey, typename TValue>
using StdHashMap = std::unordered_map<TKey, TValue>;

template <typename TContainer, typename TObject>
bool ContainerHas(const TContainer& container, const TObject& object) {
    return container.find(object) != container.end();
}

template <typename TExcept>
void check(bool condition, const TExcept& exception) {
    if (!condition) {
        throw exception;
    }
}

}  // namespace bier
