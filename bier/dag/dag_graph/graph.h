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
#include <bier/dag/dag_graph/node.h>
#include <bier/utils/iterator_range.h>

namespace bier {

class VisualOpDag {
public:
    VisualOpDagNode* AddNode();

    auto Nodes() const {
        return IteratorRange(nodes_);
    }
    void Reset() {
        nodes_.clear();
    }

private:
    std::vector<VisualOpDagNodePtr> nodes_;
};

}   // bier
