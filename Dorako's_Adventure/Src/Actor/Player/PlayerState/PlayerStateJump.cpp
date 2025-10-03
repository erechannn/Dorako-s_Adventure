#include"PlayerStateJump.h"

PlayerStateJump::PlayerStateJump(Player*owner):
	PlayerState::PlayerState(owner) {

}
void PlayerStateJump::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::JumpStart, false);
	owner_->jump(delta_time);
	if (owner_->is_motion_end()) {
		//owner_->change_state(PlayerState::StateMove);
	}
	
}
void PlayerStateJump::start() {

}
void PlayerStateJump::end() {

}