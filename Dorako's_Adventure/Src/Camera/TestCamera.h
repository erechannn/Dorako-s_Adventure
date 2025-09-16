#ifndef TEST_CAMERA_H_
#define TEST_CAMERA_H_


#include "Actor/Actor.h"

class TestCamera : public Actor {
public:
	TestCamera(IWorld* world, const GSvector3& position, const GSvector3& at);

	virtual void update(float delta_time)override;

	virtual void draw()const override;
private:
	float yaw_{ 0.0f };
	float pitch_{ 0.0f };
	GSvector2 result;

};






#endif // !TEST_CAMERA_H_
