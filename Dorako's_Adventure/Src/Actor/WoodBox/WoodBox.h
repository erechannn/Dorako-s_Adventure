#ifndef WOOD_BOX_H_
#define WOOD_BOX_H_

#include "../Character.h"

class WoodBox :public Character {
public:
	WoodBox(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other)override;
private:
	bool is_above_actor(Actor& other);
};


#endif // !WOOD_BOX_H_
