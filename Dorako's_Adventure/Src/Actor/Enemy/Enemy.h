#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"
#include "../../State/StateMachine.h"

class Enemy :public Actor {
public:
	Enemy(GSuint mesh);
	void update(float delta_time)override {}
	void draw()const override {};
	void react(Actor& other) {};
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


};





#endif // !ENEMY_H_
