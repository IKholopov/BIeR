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
#include <bier/dag/dag_graph/graph.h>
#include <bier/dag/dag_view.h>
#include <bier/serialization/text_serializer.h>

namespace bier {

class OpDagBuilder {
public:
    void Build(const bier::BasicBlock* block);

    const VisualOpDag& Graph() const {
        return graph_;
    }

private:
    StringSerializer serializer_;
    VisualOpDag graph_;
    HashMap<DagView, VisualOpDagNode*> all_nodes_;
    StdHashSet<const VisualOpDagNode*> processed_;

    void fillIn(const DagView& view, VisualOpDagNode* node) const;
    void fillOp(const DagView& view, VisualOpDagNode* node) const;
    void fillConst(const DagView& view, VisualOpDagNode* node) const;
    void fillSignature(const DagView& view, VisualOpDagNode* node) const;
    void init(const DagView& view);
    void process(const DagView& view);
    void clear();
};

}   // bier
