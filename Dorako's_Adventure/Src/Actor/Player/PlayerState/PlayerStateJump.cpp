#include"PlayerStateJump.h"

PlayerStateJump::PlayerStateJump(Player*owner):
	PlayerState::PlayerState(owner) {

}
void PlayerStateJump::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::JumpStart, false);
	//星の位置
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	//自分の位置
	GSvector3 position = owner_->transform().position();
	//上方向ベクトルを求める
	GSvector3 jump = position - planet_position;
	//単一ベクトル
	jump = jump.normalize();
	GSvector3 jump_velocity{ 0.0f,0.0f,0.0f };
	//ジャンプの移動量の設定
	jump_velocity += jump * jump_power_ * delta_time;
	//移動
	owner_->transform().translate(jump_velocity, GStransform::Space::World);
	//飛んでいる最中に横に移動が可能
	owner_->move(delta_time);
	if (owner_->is_motion_end() ) {
		owner_->change_state(PlayerState::StateFlying);
	}
}
void PlayerStateJump::start() {

}
void PlayerStateJump::end() {

}