#ifndef KURIBO_H_
#define KURIBO_H_

#include "Enemy.h"
#include "EnemyEye.h"

class Kuribo :public Enemy {
public:
	Kuribo(IWorld* world, GSvector3 position);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other)override;
	virtual void idle(float delta_time)override;
	virtual void search(float delta_time)override;
	virtual void chase(float delta_time)override;
	virtual void dead(float delta_time)override;
private:
	bool is_player_in_sight();
	GSvector3 player_position();
private:
	//視界
	EnemyEye enemy_eye_;
	//アクター上のプレイヤー
	Actor* player_{ nullptr };
	bool is_chase_{ true };
	bool is_dead_{ false };
	float idle_timer_{ 0.0f };
};



#endif // !KURIBO_H_
