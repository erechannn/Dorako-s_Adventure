#ifndef FIRE_ITEM_H_
#define FIRE_ITEM_H_

#include "../Character.h"

class FireItem :public Character {
public:
	FireItem(IWorld* world, GSvector3 position);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other)override;
	void player_react(Player& player);
};








#endif // !FIRE_ITEM_H_
