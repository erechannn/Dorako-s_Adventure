#ifndef DUMMY_PLAYER_H_
#define DUMMY_PLAYER_H_

#include"../Actor/Actor.h"

class DummyPlayer :public Actor {
public:
	DummyPlayer(IWorld*world);
	void update(float delta_time)override;
private:
	Actor* player_{nullptr};

};




#endif // !DUMMY_PLAYER_H_
