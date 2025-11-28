#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Actor.h"
#include "../State/StateMachine.h"
#include "../Stage/StageManager.h"

class Character :public Actor {
public:
	//コントラスト
	Character(GSuint mesh);
	//更新処理
	virtual void update(float delta_time)override {}
	//描画処理
	virtual void draw() const override {}
	//当たり判定
	virtual void react(Actor& other) override{}
	//状態の変更
	void change_state(int next_state);
public:
	bool is_ground();
	int now_health();
	void take_damage();
protected:
	//ステートマシン
	StateMachine state_;
	//壁との当たり判定
	void collide_field();
	//他アクターとの衝突処理
	void collide_actor(Actor& other);
	//重力
	void gravity_update(float delta_time);
	//地面との当たり判定処理
	void collide_ground();
	//重力量
	float gravity_{ -0.001 };
	//重力による移動量
	GSvector3 gravity_velocity_{ 0.0f,0.0f,0.0f };
	int max_health_{ 1 };
	int health_{ 1 };
	//足元のオフセット
	float foot_offset_{ 0.1f };
	//キャラの高さ
	float height_{ 1.0f };
	//重力を適応するか
	bool is_ground_{ false };
	bool is_zero_gravity_{ false };

	GSvector3 hit_position_{};
	GSvector3 collider_point_{};
};



#endif // !CHARACTER_H_
