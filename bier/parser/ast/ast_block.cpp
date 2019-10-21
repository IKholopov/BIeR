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
#include "ast_block.h"

namespace bier {

namespace ast {

Block::Block(std::vector<ReturnValueOpPtr>&& ops, BranchOpPtr&& branch_op,
             const std::string& label, const FilePos& pos)
    : ASTBase(pos),
      ops_(std::move(ops)),
      branch_(std::move(branch_op)),
      label_(label) {
}

}   // ast

}   // bier
