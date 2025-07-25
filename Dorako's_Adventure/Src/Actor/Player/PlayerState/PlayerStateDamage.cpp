#include"PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage(Player* owner)
	:PlayerState(owner) {

}
void PlayerStateDamage::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::Damage, false);
	if (!owner_->mesh_->is_end_motion())return;
	owner_->change_state(PlayerState::StateMove);
}
void PlayerStateDamage::start() {

}
void PlayerStateDamage::end() {

}