#ifndef PLAYER_H_
#define PLAYER_H_

#include"../Character.h"
#include"../../State/StateMachine.h"

class Player :public Character {
public:
	Player(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);
	void move(float delta_time);
	void flying(float delta_time);
	void landing(float delta_time);
	void attack();
//送る数値
public:
	//モーションの終わり
	bool is_motion_end();
	//現在の体力
	int now_health_(){}

	void consume_fire_count();

	int fire_count();

	UINT get_player_now_state();

	float get_fly_timer();

private:
	bool is_above_enemy(Actor& other);
	bool is_move_{ false };
	int fire_count_{ 3 };
	bool invincible_{ false };
	float invincible_timer_{ 0.0f };
	bool debug_invincible_{ false };
	float fly_timer_{ 180.0f };
	bool is_landing_lag_enabled{ false };
	float flying_timer_{ 0.0f };
};



#endif // !PLAYER_H_
