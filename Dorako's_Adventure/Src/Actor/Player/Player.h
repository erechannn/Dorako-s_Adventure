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
	//火の玉を減らす
	void consume_fire_count();
	//火の玉を増やす
	void add_fire_count();
	//現在の火の玉の回数を返す
	int fire_count();
	//現在のプレイヤーの状態を返す
	UINT get_player_now_state();
	//飛んでいる時間を返す
	float get_fly_timer();
	//着地隙を与えるかを返す
	bool ground_gap();

private:
	//敵が自身の下にいるか
	bool is_above_enemy(Actor& other);

	bool is_move_{ false };					//動けるか
	int fire_count_{ 3 };					//火の玉の回数
	bool invincible_{ false };				//無敵か
	float invincible_timer_{ 0.0f };		//無敵時間
	bool debug_invincible_{ false };		//デバック用の無敵
	float fly_timer_{ 180.0f };				//飛べる時間
	float flying_timer_{ 0.0f };			//飛んでいる時間
};



#endif // !PLAYER_H_
