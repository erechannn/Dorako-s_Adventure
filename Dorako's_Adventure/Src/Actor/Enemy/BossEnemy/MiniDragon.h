#ifndef MINI_DRAGON_H_
#define MINI_DRAGON_H_

#include "../Enemy.h"
#include "../../../BehaviorTree/BehaviorTree.h"
#include "../../Player/Player.h"

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

	void first_fire_attack(float delta_time)override;
//プライベートな関数
private:
	void perform_fire_attack();

	void perform_charge_attack();

	void perform_escape_action();

	void perform_melee_attack();

	void build_attack_behavior_tree();

	void look_to_player( GSvector3 target);
//プライベートな変数
private:
	std::unique_ptr<BehaviorNode>attack_behavior_tree_;
	bool has_fired_fire_attack_{ false };
	Actor* player_{ nullptr };
	float max_speed_{};
	float idle_timer_{ 0.0f };
	float player_distance_{};
	UINT now_player_state_{ 0 };
	GSvector3 saved_position_{ 0.0f,0.0f,0.0f };
	bool is_fire_attack_finished_{ false };
	int fire_attack_count_{ 0 };
	float delta_time_{};

};






#endif // !MINI_DRAGON_H_
