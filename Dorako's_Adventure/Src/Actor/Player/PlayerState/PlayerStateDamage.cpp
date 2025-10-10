#include"PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage(Player* owner)
	:PlayerState::PlayerState(owner) {

}
void PlayerStateDamage::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::Damage,false);
	if (owner_->now_health()<=0) {
		owner_->change_state(PlayerState::StateDead);
	}
	else if (owner_->is_motion_end()) {
		owner_->change_state(PlayerState::StateMove);
	}
}
void PlayerStateDamage::start() {

}
void PlayerStateDamage::end() {

}