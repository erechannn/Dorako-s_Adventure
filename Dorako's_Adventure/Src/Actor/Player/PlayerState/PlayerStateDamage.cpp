#include"PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage(Player* owner)
	:PlayerState::PlayerState(owner) {

}
void PlayerStateDamage::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::Damage,false);
	if (owner_->now_health()<=0) {						//もしプレイヤーの体力がなくなったら
		owner_->change_state(PlayerState::StateDead);	//プレイヤーの状態を死亡にする
	}
	else if (owner_->is_motion_end()&&owner_->is_ground()) {					//それ以外でダメージモーションが終わったら
		owner_->change_state(PlayerState::StateMove);	//状態を元に戻す
	}
}
void PlayerStateDamage::start() {

}
void PlayerStateDamage::end() {

}