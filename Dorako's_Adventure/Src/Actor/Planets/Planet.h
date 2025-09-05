#ifndef PLANET_H_
#define PLANET_H_

#include"../Actor.h"

class Planet :public Actor {
public:
	Planet(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);

};




#endif // !PLANET_H_
