#include "CameraRotateAround.h"
#include "World/IWorld.h"
#include "Shape/Line.h"
#include "World/Field.h"

#include <iostream>


const GSvector3 PlayerOffset{ 0.0f,0.0f,-7.0f };

const GSvector3 ReferencePointOffset{ 0.0f,2.0f,0.0f };

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
	//�X�e�B�b�N�̈ړ��ʂ̐��l��
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);
	Actor* dummy_player = world_->find_actor("DummyPlayer");
	if (dummy_player == nullptr)return;
	player_up_ = dummy_player->transform().up();
	if (gsGetKeyState(GKEY_LEFT)) yaw_ += 3.0f * delta_time;
	if (gsGetKeyState(GKEY_RIGHT))yaw_ -= 3.0f * delta_time;
	//�E�X�e�B�b�N�̈ړ��ʂ���J�����̈ړ�
	if (result.x < 0.0f) yaw_ += 2.0f * result_normalize * delta_time;
	if (result.x > 0.0f)yaw_ -= 2.0f * result_normalize * delta_time;
	if (result.y < 0.0f)pitch_ += 1.0f * result_normalize * delta_time;
	if (result.y > 0.0f) pitch_ -= 1.0f * result_normalize * delta_time;
	pitch_ = CLAMP(pitch_, -70.0f, 30.0f);

	GSvector3 at = dummy_player->transform().position()+ReferencePointOffset;
	//�s�b�`�ƃ��E�̒P��x�N�g��
	GSvector3 view = GSvector3::createFromPitchYaw(pitch_, yaw_)*10.0f;
	//�v���C���[�̏�����x�N�g�������ɉ�]
	GSvector3 position = dummy_player->transform().transformPoint(view);
	//�t�B�[���h�Ƃ̓����蔻��
	Line line{ at, position };
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		//���������ʒu�ɃJ�����̈ʒu��␳����
		position = intersect;
	}
	//�J�����̈ړ�
	transform_.position(position);
	transform_.lookAt(at,dummy_player->transform().up());

}
void CameraRotateAround::draw()const {
	GSvector3 eye = transform_.position();
	GSvector3 at = eye + transform_.forward();
	GSvector3 up = transform_.up();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.x, eye.y, eye.z,     // ���_�̈ʒu
		at.x, at.y, at.z,     // �����_�̈ʒu
		up.x, up.y, up.z      // ���_�̏����
	);

}