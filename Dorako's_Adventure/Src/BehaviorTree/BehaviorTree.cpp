#include "BehaviorTree.h"

//アクションノードの実装
ActionNode::ActionNode(std::function<Status()>func) :action_(func) {}
Status ActionNode::tick() { return action_(); }

//コンディションノードの実装
ConditionNode::ConditionNode(std::function<bool()>func):condition_(func){}
Status ConditionNode::tick() { return condition_() ? Status::Success : Status::Failure; }//trueだったらSuccessをfalseだったらFailureを返す

//コンポジットノード（ANDロジック）の実装
SequenceNode::SequenceNode():current_child_(0){}
//条件の追加
void SequenceNode::add_child(std::unique_ptr<BehaviorNode> child) {
	children_.push_back(std::move(child));
}
//その条件の状態を返す
Status SequenceNode::tick() {
	while (current_child_ < children_.size()) {
		Status status = children_[current_child_]->tick();
		if (status == Status::Failure) {
			current_child_ = 0;
			return Status::Failure;
		}
		if (status == Status::Running)return Status::Running;
		current_child_++;
	}
	current_child_ = 0;
	return Status::Success;
}

//コンポジットノード（ORロジック）の実装
SelectorNode::SelectorNode():current_child_(0){}
//条件の追加
void SelectorNode::add_child(std::unique_ptr<BehaviorNode>child) {
	children_.push_back(std::move(child));
}
//条件の状態を返す
Status SelectorNode::tick() {
	while (current_child_ < children_.size()) {
		Status status = children_[current_child_]->tick();
		if (status == Status::Success) {
			current_child_ = 0;
			return Status::Success;
		}
		if (status == Status::Running) return Status::Running;
		current_child_++;
	}
	current_child_ = 0;
	return Status::Failure;
}