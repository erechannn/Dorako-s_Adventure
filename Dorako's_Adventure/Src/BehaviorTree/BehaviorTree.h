#ifndef BEHAVIOR_TREE_H_
#define BEHAVIOR_TREE_H_

#include <vector>
#include <memory>
#include <functional>

//各ノードの状態
enum class Status {
    Success,//成功
    Failure,//失敗
    Running //継続中
};

//実行結果を返すためのクラス（各クラスで継承する）
class BehaviorNode {
public:
    virtual ~BehaviorNode() = default;
    
    virtual Status tick() = 0;
};

//アクションノード　具体的な行動を実行するリーフノード
class ActionNode :public BehaviorNode {
public:
    explicit ActionNode(std::function<Status()>func);
    Status tick()override;
private:
    std::function<Status()> action_;
};

//コンディションノード　行動する条件をチェックするリーフノード
class ConditionNode :public BehaviorNode {
public:
    explicit ConditionNode(std::function<bool()>func);
    Status tick()override;
private:
    std::function<bool()> condition_;
};
//コンポジットノード（ANDロジック）
class SequenceNode :public BehaviorNode {
public:
    SequenceNode();
    void add_child(std::unique_ptr<BehaviorNode>child);
    Status tick()override;
private:
    std::vector<std::unique_ptr<BehaviorNode>> children_;
    size_t current_child_;
};
//コンポジットノード（ORロジック）
class SelectorNode :public BehaviorNode {
public:
    SelectorNode();
    void add_child(std::unique_ptr<BehaviorNode>child);
    Status tick()override;
private:
    std::vector<std::unique_ptr<BehaviorNode>> children_;
    size_t current_child_;
};
#endif // !BEHAVIOR_TREE_H_
