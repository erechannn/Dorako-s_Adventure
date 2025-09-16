#include "CameraRotateAround.h"
#include "World/IWorld.h"
#include "Shape/Line.h"
#include "World/Field.h"

#include <iostream>


const GSvector3 PlayerOffset{ 0.0f,0.0f,-7.0f };

const GSvector3 ReferencePointOffset{ 0.0f,1.0f,0.0f };

CameraRotateAround::CameraRotateAround(IWorld* world,
	const GSvector3& position, const GSvector3& at) {
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);
	pitch_ = (at - position).getPitch();
	yaw_ = (at - position).getYaw();
}
void CameraRotateAround::update(float delta_time) {
	gsXBoxPadGetRightAxis(0, &result);
	//スティックの移動量の数値化
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;
	transform_.parent(player->transform().parent());
	if (gsGetKeyState(GKEY_LEFT)) yaw_ += 3.0f * delta_time;
	if (gsGetKeyState(GKEY_RIGHT))yaw_ -= 3.0f * delta_time;
	//右スティックの移動量からカメラの移動
	if (result.x < 0.0f) yaw_ += 2.0f * result_normalize * delta_time;
	if (result.x > 0.0f)yaw_ -= 2.0f * result_normalize * delta_time;
	if (result.y < 0.0f)pitch_ += 1.0f * result_normalize * delta_time;
	if (result.y > 0.0f) pitch_ -= 1.0f * result_normalize * delta_time;
	pitch_ = CLAMP(pitch_, -30.0f, 45.0f);

	GSvector3 at = player->transform().position() + ReferencePointOffset;
	GSvector3 position = at + GSquaternion::euler(pitch_, yaw_, 0.0f) * PlayerOffset;
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}

	transform_.position(position);
	transform_.lookAt(at);

}
void CameraRotateAround::draw()const {
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