#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Character.h"
#include "../../State/StateMachine.h"

class Enemy :public Character {
public:
	Enemy(GSuint mesh);
	virtual void update(float delta_time)override {}
	virtual void draw()const override {}
	virtual void react(Actor& other) {}
	virtual void idle(float delta_time) {}
	virtual void search(float delta_time) {}
	virtual void chase(float delta_time) {}
	virtual void attack(float delta_time) {}
	virtual void damage(float delta_time) {}
	virtual void dead(float delta_time) {}

	virtual void first_fire_attack(float delta_time){}
protected:
	bool is_above_player(Actor& player);
	void to_target(float delta_time,GSvector3 target);
	void set_next_point();
	void up_vector_update();
protected:
	int max_health_{ 1 };
	int health_{ 1 };
	float walk_speed_{ 0.05f };
	GSvector3 target_point_{};
	GStransform first_transform_{};
	GSvector3 base_position_{};

};





#endif // !ENEMY_H_
