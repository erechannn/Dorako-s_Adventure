#ifndef KURIBO_H_
#define KURIBO_H_

#include "Enemy.h"

class Kuribo :public Enemy {
public:
	Kuribo(IWorld* world, GSvector3 position);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other)override;
	virtual void search(float delta_time)override;

};



#endif // !KURIBO_H_
