#ifndef PLAYER_H_
#define PLAYER_H_

#include"../Actor.h"

class Player :public Actor {
public:
	Player(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);
};



#endif // !PLAYER_H_
