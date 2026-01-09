#ifndef STAGE_SELECT_DOOR_H_
#define STAGE_SELECT_DOOR_H_

#include "Actor.h"


class StageSelectDoor :public Actor {
public:
	StageSelectDoor(IWorld* world, GSvector3 position,int stage_id);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other)override;
public:
	int get_stage_id();
	bool is_into_the_door();
private:
	bool is_unlock_{ false };
	int stage_id_{};
	bool is_into_the_door_{ false };
};






#endif // !STAGE_SELECT_DOOR_H_
