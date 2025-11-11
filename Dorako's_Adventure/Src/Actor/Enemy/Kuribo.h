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
	virtual void search(float delta_time)override;
private:
	void set_next_point();
	bool is_player_in_sight();
private:
	bool is_above_player(Actor& other);
	EnemyEye enemy_eye_;
	GSvector3 target_point_{ 0.0f,0.0f,0.0f };
	GSvector3 first_position_;
	GSvector3 first_forward_{};
	GSvector3 first_right_{};
	float walk_timer_{ 0.0f };
	bool is_move_{ true };
	bool undead_{ true };
	GSvector3 planet_to_target_{};
	BoundingSphere test_{};
	BoundingSphere test2_{};
	GStransform first_transform_{};
	Actor* player_{ nullptr };
};



#endif // !KURIBO_H_
