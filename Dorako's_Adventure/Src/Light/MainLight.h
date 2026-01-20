#ifndef LIGHT_H_
#define LIGHT_H_

#include "Actor/Actor.h"

class Light :public Actor {
public:
	Light(IWorld* world);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
private:
	Actor* camera_{ nullptr };
	GSvector3 camera_position_{};

};


#endif // !LIGHT_H_H
