#include "MainCamera.h"
#include "../World/IWorld.h"
#include "../World/Field.h"
#include "../Shape/Line.h"

const GSvector3 PlayerOffset{ 0.0f,3.0f,-5.0f };

const GSvector3 ReferencePointOffset{ 0.0f,1.7f,0.0f };

MainCamera::MainCamera(IWorld* world, const GSvector3& position, const GSvector3& at) {
	world_ = world;
	tag_ = "CameraTag";
	name_ = "Camera";
	transform_.position(position);
	transform_.lookAt(at);
}
void MainCamera::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if (player == nullptr)return;
	transform_.parent(player->transform().parent());
	GSvector3 position = PlayerOffset * player->transform().localToWorldMatrix();
	GSvector3 at = player->transform().position() + ReferencePointOffset;
	Line line(at, position);
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		position = intersect;
	}
	const float SmoothTime{ 12.0f };
	const float MaxSpeed{ 1.0f };
	position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
		SmoothTime, MaxSpeed, delta_time);
	transform_.position(position);
	transform_.lookAt(at);
}
void MainCamera::draw()const {
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // 視点の位置
		at.x, at.y, at.z,      // 注視点の位置
		0.0f, 1.0f, 0.0f       // 視点の上方向
	);

}