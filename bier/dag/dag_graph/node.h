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
#include <optional>
#include <string>
#include <vector>

namespace bier {

class VisualOpDagNode {
public:
    void AddAttribute(const std::string& attribute) {
        attributes_.emplace_back(attribute);
    }
    void AddDependency(const VisualOpDagNode* dependency) {
        dependencies_.push_back(dependency);
    }
    void SetSeqLink(const VisualOpDagNode* node) {
        seq_link_ = node;
    }

    const std::vector<std::string>& Attributes() const {
        return attributes_;
    }

    const std::vector<const VisualOpDagNode*>& Dependencies() const {
        return dependencies_;
    }

    std::optional<const VisualOpDagNode*> SeqLink() const {
        return seq_link_;
    }

private:
    std::vector<std::string> attributes_;
    std::vector<const VisualOpDagNode*> dependencies_;
    std::optional<const VisualOpDagNode*> seq_link_;
};

using VisualOpDagNodePtr = std::unique_ptr<VisualOpDagNode>;

}   // bier
