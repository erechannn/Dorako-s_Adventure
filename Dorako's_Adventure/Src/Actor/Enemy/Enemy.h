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
protected:
	int max_health_{ 1 };
	int health_{ 1 };
	float walk_speed_{ 0.15f };


};





#endif // !ENEMY_H_
