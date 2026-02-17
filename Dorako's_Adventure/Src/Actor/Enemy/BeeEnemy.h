#ifndef BEE_ENEMY_H_
#define BEE_ENEMY_H_

#include "Enemy.h"

class BeeEnemy :public Enemy {
public:
	BeeEnemy(IWorld* world, GSvector3 position);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other)override;
	virtual void idle(float delte_time)override;
	virtual void search(float delta_time)override;
};




#endif // !BEE_ENEMY_H_
