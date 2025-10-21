#include "ModelPlayer.h"
#include "Player/PlayerMotion.h"
#include "../World/IWorld.h"
#include "../Assets.h"

const float WalkSpeed{ 0.15f };

ModelPlayer::ModelPlayer(IWorld* world, GSvector3 position) :
	Character{ Mesh_Player } {
	world_ = world;
	name_ = "ModelPlayer";
	tag_ = "ModelTag";
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	mesh_->change_motion(PlayerMotion::Run,true);
}
void ModelPlayer::update(float delta_time) {
	mesh_->update(delta_time);
	//�t�B�[���h�Ƃ̓����蔻��
	collide_field();
	gravity_update(delta_time);
	//�v���C���[��n�ʂɑ΂��Đ����ɗ�������
	collide_ground();
	//���[���h�ϊ��s���ݒ�
	mesh_->transform(transform_.localToWorldMatrix());

	velocity_ = {0.0f, 0.0f,  WalkSpeed * delta_time };


	transform_.translate(velocity_);
}
void ModelPlayer::draw()const {
	mesh_->draw();
}
void ModelPlayer::react(Actor& other) {

}