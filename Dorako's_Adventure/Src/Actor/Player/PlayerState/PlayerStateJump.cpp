#include"PlayerStateJump.h"

PlayerStateJump::PlayerStateJump(Player*owner):
	PlayerState::PlayerState(owner) {

}
void PlayerStateJump::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::JumpStart, false);
	GSvector3 planet_position{ 0.0f,-20.0f,0.0f };//���̒��S
	GSvector3 position = owner_->transform().position();//�����̈ʒu
	GSvector3 jump = position - planet_position;//�����x�N�g�������߂�
	jump = jump.normalize();//�P��x�N�g��
	GSvector3 jump_velocity{ 0.0f,0.0f,0.0f };
	jump_velocity += jump * jump_power_ * delta_time;
	owner_->transform().translate(jump_velocity, GStransform::Space::World);//�ړ�
	if (owner_->is_motion_end() ) {
		owner_->change_state(PlayerState::StateFlying);
	}
}
void PlayerStateJump::start() {

}
void PlayerStateJump::end() {

}