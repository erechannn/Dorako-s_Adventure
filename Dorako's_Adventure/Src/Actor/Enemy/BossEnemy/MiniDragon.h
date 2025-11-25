#ifndef MINI_DRAGON_H_
#define MINI_DRAGON_H_

#include "../Enemy.h"
#include "../../../BehaviorTree/BehaviorTree.h"

class MiniDragon :public Enemy {
public:
	MiniDragon(IWorld* wworld, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other)override;
	void idle(float delta_time)override;
	void chase(float delta_time)override;
	void attack(float delta_time)override;
	void damage(float delta_time)override;
	void dead(float delta_time)override;
//プライベートな関数
private:
	void perform_fire_attack();

	void perform_charge_attack();

	void perform_escape_action();

	void build_attack_behavior_tree();
//プライベートな変数
private:
	std::unique_ptr<BehaviorNode>attack_behavior_tree_;
	bool has_fired_fire_attack_{ false };
	Actor* player_{ nullptr };
	float max_speed_{};
	float idle_timer_{ 0.0f };

};






#endif // !MINI_DRAGON_H_
