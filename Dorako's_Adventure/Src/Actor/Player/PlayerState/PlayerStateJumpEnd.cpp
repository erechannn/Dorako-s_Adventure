#include"PlayerStateJumpEnd.h"

PlayerStateJumpEnd::PlayerStateJumpEnd(Player* owner) :
	PlayerState(owner) {

}

void PlayerStateJumpEnd::update(float delta_time) {
	//’…’nŒ„‚ð—^‚¦‚é‚©
	if (owner_->ground_gap()) {
		owner_->mesh_->change_motion(PlayerMotion::JumpEnd, false);
		if (owner_->is_motion_end()) {
			owner_->landing(delta_time);
			owner_->change_state(PlayerState::StateMove);
		}
	}
	else {
		owner_->change_state(PlayerState::StateMove);
		owner_->landing(delta_time);

	}
}
void PlayerStateJumpEnd::start() {

}
void PlayerStateJumpEnd::end() {

}