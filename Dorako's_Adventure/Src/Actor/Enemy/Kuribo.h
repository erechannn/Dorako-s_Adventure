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
private:
	bool is_player_in_sight();
private:
	EnemyEye enemy_eye_;
	GSvector3 first_position_{};
	float walk_timer_{ 0.0f };
	bool is_move_{ true };
	bool undead_{ true };
	GSvector3 planet_to_target_{};
	BoundingSphere test_{};
	BoundingSphere test2_{};
	Actor* player_{ nullptr };
	bool is_chase_{ false };
};



#endif // !KURIBO_H_
