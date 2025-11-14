#ifndef MINI_DRAGON_H_
#define MINI_DRAGON_H_

#include "../Enemy.h"

class MiniDragon :public Enemy {
public:
	MiniDragon(IWorld* wworld, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other)override;

};






#endif // !MINI_DRAGON_H_
