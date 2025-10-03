#include"PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage(Player* owner)
	:PlayerState::PlayerState(owner) {

}
void PlayerStateDamage::update(float delta_time) {
	if (owner_->is_motion_end()) {
		owner_->change_state(PlayerState::StateMove);
	}
}
void PlayerStateDamage::start() {

}
void PlayerStateDamage::end() {

}