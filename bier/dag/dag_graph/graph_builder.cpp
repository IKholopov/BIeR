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
#include "graph_builder.h"
#include <sstream>

namespace bier {

void OpDagBuilder::Build(const BasicBlock* block) {
    clear();
    DagView root_view = DagView::Root(block);
    process(root_view);
}

void OpDagBuilder::fillIn(const DagView& view, VisualOpDagNode* node) const {
    const DagNodeType type = view.GetType();
    switch (type) {
        case DagNodeType::SIGNATURE:
            node->AddAttribute("signature");
        break;
        case DagNodeType::CONST:
            node->AddAttribute("const");
        break;
        case DagNodeType::STATIC_DATA:
            node->AddAttribute("static data");
        break;
        case DagNodeType::ARG:
            node->AddAttribute("arg");
        break;
        case DagNodeType::ALLOCA:
            node->AddAttribute("alloca");
        break;
        case DagNodeType::MUTABLE:
            node->AddAttribute("mutable");
        break;
        case DagNodeType::OPERATION:
            node->AddAttribute("op");
            fillOp(view, node);
        break;
        case DagNodeType::EXTERNAL_OPERATION:
            node->AddAttribute("external op");
        break;
    }
    if (type != DagNodeType::EXTERNAL_OPERATION
            && type != DagNodeType::OPERATION) {
        node->AddAttribute(view.AsVal()->GetName());
    }
}

void OpDagBuilder::fillOp(const DagView& view, VisualOpDagNode* node) const {
    assert(view.GetType() == DagNodeType::OPERATION);
    std::ostringstream stream;
    serializer_.TranslateOp(view.AsOp(), stream);
    node->AddAttribute(stream.str());
}

void OpDagBuilder::init(const DagView& view) {
    VisualOpDagNode* node = graph_.AddNode();
    all_nodes_.insert({view, node});
    fillIn(view, node);
}

void OpDagBuilder::process(const DagView& view)
{
    if (!ContainerHas(all_nodes_, view)) {
        init(view);
    }
    VisualOpDagNode* node = all_nodes_.at(view);
    if (ContainerHas(processed_, node)) {
        return;
    }
    processed_.insert(node);

    auto link = view.SeqLink();
    if (link.has_value()) {
        process(link.value());
        node->SetSeqLink(all_nodes_.at(link.value()));
    }

    for (const auto& dependency : view.Links()) {
        process(dependency);
        node->AddDependency(all_nodes_.at(dependency));
    }
}

void OpDagBuilder::clear() {
    graph_.Reset();
    all_nodes_.clear();
    processed_.clear();
}

}   // bier
