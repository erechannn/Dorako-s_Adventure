#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"

class Enemy :public Actor {
public:
	Enemy(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);

};





#endif // !ENEMY_H_
