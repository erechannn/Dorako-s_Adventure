#include"PlayerStateJumpEnd.h"

PlayerStateJumpEnd::PlayerStateJumpEnd(Player* owner) :
	PlayerState(owner) {

}

void PlayerStateJumpEnd::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::JumpEnd,false);
	if (owner_->is_motion_end()) {
		owner_->change_state(PlayerState::StateMove);
	}
}
void PlayerStateJumpEnd::start() {

}
void PlayerStateJumpEnd::end() {

}