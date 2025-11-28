#ifndef COIN_H_
#define COIN_H_

#include "../Character.h"

class Coin :public  Character{
public:
	Coin(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other)override;
};







#endif // !COIN_H_
