#ifndef CAMERA_ROTATE_AROUND_H_
#define CAMERA_ROTATE_AROUND_H_

#include "Actor/Actor.h"

class CameraRotateAround : public Actor {
public:
	CameraRotateAround(IWorld* world, const GSvector3& position, const GSvector3& at);

	virtual void update(float delta_time)override;

	virtual void draw()const override;
private:
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
	GSvector3 view_{ 0.0f,0.0f,0.0f };
	GSvector2 result;
	GSvector3 player_up_{ 0.0f,0.0f,0.0f };
	GSvector3 first_position_{};

};

#endif