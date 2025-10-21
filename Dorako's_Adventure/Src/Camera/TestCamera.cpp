#include"TestCamera.h"
#include "World/IWorld.h"
#include "Shape/Line.h"
#include "World/Field.h"

#include <iostream>

const GSvector3 PlayerOffset{ 0.0f,-20.0f,-20.0f };

TestCamera::TestCamera(IWorld* world, const GSvector3& position, const GSvector3& at) {
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);
	pitch_ = (at - position).getPitch();
	yaw_ = (at - position).getYaw();
}
void TestCamera::update(float delta_time) {
	if (gsGetKeyState(GKEY_LEFT)) yaw_ += 1.0f * delta_time;
	if (gsGetKeyState(GKEY_RIGHT))yaw_ -= 1.0f * delta_time;
	if (gsGetKeyState(GKEY_UP))pitch_ += 1.0f * delta_time;
	if (gsGetKeyState(GKEY_DOWN))pitch_ -= 1.0f * delta_time;
	
	GSvector3 at{ 0.0f,4.0f,0.0f };
	GSvector3 position = at + GSquaternion::euler(0.0f, yaw_, 0.0f)*PlayerOffset;

	velocity_.y = pitch_;

	transform_.position(position);
	transform_.translate(velocity_, GStransform::Space::World);
	transform_.lookAt(at);
}
void TestCamera::draw()const {
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	GSvector3 up = transform_.up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,     // 注視点の位置
		up.x, up.y, up.z      // 視点の上方向
	);

}