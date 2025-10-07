#include"PlayerStateJump.h"

PlayerStateJump::PlayerStateJump(Player*owner):
	PlayerState::PlayerState(owner) {

}
void PlayerStateJump::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::JumpStart, false);
	GSvector3 planet_position{ 0.0f,-20.0f,0.0f };//星の中心
	GSvector3 position = owner_->transform().position();//自分の位置
	GSvector3 jump = position - planet_position;//方向ベクトルを求める
	jump = jump.normalize();//単一ベクトル
	GSvector3 jump_velocity{ 0.0f,0.0f,0.0f };
	jump_velocity += jump * jump_power_ * delta_time;
	owner_->transform().translate(jump_velocity, GStransform::Space::World);//移動
	if (owner_->is_motion_end() ) {
		owner_->change_state(PlayerState::StateFlying);
	}
}
void PlayerStateJump::start() {

}
void PlayerStateJump::end() {

}