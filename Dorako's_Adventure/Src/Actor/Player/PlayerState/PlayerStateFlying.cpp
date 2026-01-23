#include "PlayerStateFlying.h"

PlayerStateFlying::PlayerStateFlying(Player* owner) :
	PlayerState(owner) {

}

void PlayerStateFlying::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::Flying);
	owner_->flying(delta_time);
	//’…’n‚µ‚½‚çó‘Ô•Ï‰»
	if (owner_->is_ground()) {
		owner_->change_state(PlayerState::StateJumpEnd);
	}
}
void PlayerStateFlying::start() {
	flying_timer_ = 0.0f;
}
void PlayerStateFlying::end() {

}