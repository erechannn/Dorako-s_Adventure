#include "BehaviorTree.h"

// ============================================
// アクションノードの実装
// ============================================
ActionNode::ActionNode(std::function<Status()> func)
    : action_(func) {
}

Status ActionNode::tick() {
    return action_();
}

// ============================================
// コンディションノードの実装
// ============================================
ConditionNode::ConditionNode(std::function<bool()> func)
    : condition_(func) {
}

Status ConditionNode::tick() {
    // trueならSuccess、falseならFailureを返す
    return condition_() ? Status::Success : Status::Failure;
}

// ============================================
// シーケンスノード（ANDロジック）の実装
// すべての子ノードが成功して初めて成功
// ============================================
SequenceNode::SequenceNode()
    : current_child_(0) {
}

void SequenceNode::add_child(std::unique_ptr<BehaviorNode> child) {
    children_.push_back(std::move(child));
}

Status SequenceNode::tick() {
    // 現在の子ノードから順に実行
    for (; current_child_ < children_.size(); current_child_++) {
        Status status = children_[current_child_]->tick();

        // 1つでも失敗したら全体が失敗
        if (status == Status::Failure) {
            current_child_ = 0;
            return Status::Failure;
        }

        // 実行中なら状態を保持して待機
        if (status == Status::Running) {
            return Status::Running;
        }

        // Success の場合は次の子ノードへ継続
    }

    // すべての子ノードが成功
    current_child_ = 0;
    return Status::Success;
}

// ============================================
// セレクターノード（ORロジック）の実装
// いずれかの子ノードが成功すれば成功
// ============================================
SelectorNode::SelectorNode()
    : current_child_(0) {
}

void SelectorNode::add_child(std::unique_ptr<BehaviorNode> child) {
    children_.push_back(std::move(child));
}

Status SelectorNode::tick() {
    // 現在の子ノードから順に実行
    for (; current_child_ < children_.size(); current_child_++) {
        Status status = children_[current_child_]->tick();

        // 1つでも成功したら全体が成功
        if (status == Status::Success) {
            current_child_ = 0;
            return Status::Success;
        }

        // 実行中なら状態を保持して待機
        if (status == Status::Running) {
            return Status::Running;
        }

        // Failure の場合は次の子ノードへ継続
    }

    // すべての子ノードが失敗
    current_child_ = 0;
    return Status::Failure;
}