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

#include <bier/core/basic_block.h>

namespace bier {

class DagContext;

using DagContextPtr = std::shared_ptr<DagContext>;

class DagContext {
public:
    using DependentOps = std::vector<BasicBlock::ConstOperationIterator>;
    void Build(const BasicBlock* block);

    bool Has(const Operation* op) const;
    BasicBlock::ConstOperationIterator Get(const Operation* op) const;
    const DependentOps& GetDependent(const Operation* op) const;
    const BasicBlock* GetBlock() const {
        return block_;
    }

    static DagContextPtr Make(const BasicBlock* block);

private:
    StdHashMap<const Operation*, BasicBlock::ConstOperationIterator> op_to_iterator_;
    StdHashMap<const Operation*, DependentOps> op_dependent_;
    const BasicBlock* block_ = nullptr;
    DependentOps empty_;
};

}   // bier

