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
#include "dot_serializer.h"
#include <bier/common.h>

namespace bier {

namespace {

struct Edge {
    std::string node;
    const VisualOpDagNode* from;
    const VisualOpDagNode* to;

    Edge(const std::string& _node, const VisualOpDagNode* _from, const VisualOpDagNode* _to)
        : node(_node), from(_from), to(_to) {
    }
};

}   // namespace

DagDotSerializer::DagDotSerializer(std::ostream& dag_stream)
    : stream_(dag_stream) {
    stream_ << "digraph {\n"
            << "rankdir=\"BT\";\n";
}

DagDotSerializer::~DagDotSerializer() {
    stream_ << "}\n";
}

void DagDotSerializer::Serialize(const VisualOpDag* graph, const std::string& label) {
    std::vector<Edge> seq_links;
    std::vector<Edge> edges;
    StdHashMap<const VisualOpDagNode*, int> node_to_idx;
    int counter = 0;

    stream_ << "subgraph cluster_" << std::to_string(++subgraphs_counter_) << " {\n";
    stream_ << "label=\"" << label << "\";\n";
    for (const auto& node : graph->Nodes()) {
        int id = counter++;
        node_to_idx.insert({node.get(), id});
        stream_ << GetNodeName(id) << " [shape=record,shape=Mrecord,label=\"{"
                << "{<seq>seq";
        for (size_t i = 0; i < node->Dependencies().size(); ++i) {
            edges.emplace_back(std::to_string(i), node.get(), node->Dependencies().at(i));
            stream_ << "|" << "<" << i << ">" << i;
        }
        stream_ << "}|" << AttributesString(node.get()) << "}\"];\n";
        if (node->SeqLink().has_value()) {
            seq_links.emplace_back("seq", node.get(), node->SeqLink().value());
        }
    }


    for (const auto& edge : seq_links) {
        stream_ << GetNodeName(node_to_idx.at(edge.from)) << ":" << edge.node << " -> "
                << GetNodeName(node_to_idx.at(edge.to)) << "[color=blue,style=dashed];\n";
    }
    for (const auto& edge : edges) {
        stream_ << GetNodeName(node_to_idx.at(edge.from)) << ":" << edge.node << " -> "
                << GetNodeName(node_to_idx.at(edge.to)) << ";\n";
    }

    stream_ << "}\n";
}

std::string DagDotSerializer::AttributesString(const VisualOpDagNode* node) const {
    std::string attribute_string;
    const auto& attributes = node->Attributes();
    for (auto it = attributes.begin(); it != attributes.end(); ++it) {
        attribute_string += *it;
        if (it + 1 != attributes.end()) {
            attribute_string += "|";
        }
    }

    return attribute_string;
}

std::string DagDotSerializer::GetNodeName(int index) const {
    return "node_" + std::to_string(subgraphs_counter_) + "_" + std::to_string(index);
}



}   // bier
