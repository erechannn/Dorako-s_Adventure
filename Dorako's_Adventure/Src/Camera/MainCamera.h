#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"

class MainCamera :public Actor {
public:
	MainCamera(IWorld* world, const GSvector3& position, const GSvector3& at);

	virtual void update(float delta_time)override;
	virtual void draw()const override;
};


#endif
