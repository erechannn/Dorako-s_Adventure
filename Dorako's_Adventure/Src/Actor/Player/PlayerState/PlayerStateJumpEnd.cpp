#include"PlayerStateJumpEnd.h"

PlayerStateJumpEnd::PlayerStateJumpEnd(Player* owner) :
	PlayerState(owner) {

}

void PlayerStateJumpEnd::update(float delta_time) {
	owner_->landing(delta_time);
	if (owner_->ground_gap()) {
		owner_->mesh_->change_motion(PlayerMotion::JumpEnd, false);
		if (owner_->is_motion_end()) {
			owner_->change_state(PlayerState::StateMove);
		}
	}
	else owner_->change_state(PlayerState::StateMove);
}
void PlayerStateJumpEnd::start() {

}
void PlayerStateJumpEnd::end() {

}