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
#include <bier/parser/ast/ast_node.h>

namespace bier {

namespace ast {

class OperationNode : public ASTNode {
public:
    virtual const Operation& Op() const = 0;
};

using OperationNodePtr = std::unique_ptr<OperationNode>;

class ReturnValueOp {
public:
    virtual ~ReturnValueOp() = default;
    virtual void SetReturnValue(const VarEntry& entry) = 0;
};

using ReturnValueOpPtr = std::unique_ptr<ReturnValueOp>;

class RegularOp : public ASTBase<RegularOp, OperationNode>, public ReturnValueOp {
public:
    RegularOp(Operation&& op, const FilePos& pos)
        : ASTBase(pos),
          op_(std::move(op)) {
    }

    const Operation& Op() const override {
        return op_;
    }
    void SetReturnValue(const VarEntry& entry) override {
        op_.return_var = entry;
    }

private:
    Operation op_;
};

using RegularOpPtr = std::unique_ptr<RegularOp>;

class GepOp : public ASTBase<GepOp, OperationNode>, public ReturnValueOp  {
public:
    explicit GepOp(Operation&& op, const std::string& name,
                   uint64_t index, const FilePos& pos)
        : ASTBase(pos),
          op_(std::move(op)),
          layout_(name),
          index_(index) {
    }
    explicit GepOp(Operation&& op, Layout&& layout,
                   uint64_t index, const FilePos& pos)
        : ASTBase(pos),
          op_(std::move(op)),
          layout_(std::move(layout)),
          index_(index) {
    }

    const Operation& Op() const override {
        return op_;
    }
    void SetReturnValue(const VarEntry& entry) override {
        op_.return_var = entry;
    }

    uint64_t GetIndex() const {
        return index_;
    }

    bool IsAnonymous() const {
        return layout_.index() == kLayoutIndex;
    }

private:
    static constexpr int kLayoutIndex = 1;

    Operation op_;
    std::variant<std::string, Layout> layout_;
    uint64_t index_ = 0;
};


class AllocLayoutOp : public ASTBase<GepOp, OperationNode>, public ReturnValueOp  {
public:
    explicit AllocLayoutOp (Operation&& op, const std::string& name,
                   uint64_t count, const FilePos& pos)
        : ASTBase(pos),
          op_(std::move(op)),
          layout_(name),
          count_(count) {
    }
    explicit AllocLayoutOp(Operation&& op, Layout&& layout,
                   uint64_t count, const FilePos& pos)
        : ASTBase(pos),
          op_(std::move(op)),
          layout_(std::move(layout)),
          count_(count) {
    }

    const Operation& Op() const override {
        return op_;
    }
    void SetReturnValue(const VarEntry& entry) override {
        op_.return_var = entry;
    }

    uint64_t GetCount() const {
        return count_;
    }

    bool IsAnonymous() const {
        return layout_.index() == kLayoutIndex;
    }

private:
    static constexpr int kLayoutIndex = 1;

    Operation op_;
    std::variant<std::string, Layout> layout_;
    uint64_t count_ = 0;
};


class BranchOp : public ASTBase<BranchOp, OperationNode> {
public:
    BranchOp(Operation&& op, std::vector<std::string>&& labels, const FilePos& pos)
        : ASTBase(pos),
          op_(std::move(op)),
          labels_(std::move(labels)) {
    }

    const Operation& Op() const override {
        return op_;
    }

private:
    Operation op_;
    std::vector<std::string> labels_;
};

using BranchOpPtr = std::unique_ptr<BranchOp>;

class Block : public ASTBase<Block>{
public:
    Block(std::vector<ReturnValueOpPtr>&& ops, BranchOpPtr&& branch_op,
          const std::string& label, const FilePos& pos);

private:
    std::vector<ReturnValueOpPtr> ops_;
    BranchOpPtr branch_;
    std::string label_;
};

using BlockPtr = std::unique_ptr<Block>;

}   // ast

}   // bier
