#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"
#include "../../State/StateMachine.h"

class Enemy :public Actor {
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
	void change_state(int next_state);
protected:
	StateMachine state_;
	void collide_field();
	void collide_actor(Actor& other);
	void gravity_update(float delta_time);
	void collide_ground();
	float gravity_{ -0.003 };
	GSvector3 gravity_velocity_{ 0.0f,0.0f,0.0f };
	int max_health_{ 1 };
	int health_{ 1 };
	float foot_offset_{ 0.1f };
	float enemy_height_{ 1.0f };
	float walk_speed_{ 0.15f };


};





#endif // !ENEMY_H_
