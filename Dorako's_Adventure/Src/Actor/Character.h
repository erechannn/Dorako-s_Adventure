#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Actor.h"
#include "../State/StateMachine.h"

class Character :public Actor {
public:
	Character(GSuint mesh);
	virtual void update(float delta_time)override {}
	virtual void draw() const override {}
	virtual void react(Actor& other) override{}
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
	float height_{ 1.0f };
	bool is_ground_{ false };
};



#endif // !CHARACTER_H_
