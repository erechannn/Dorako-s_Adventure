#include"PlayerStateJump.h"

PlayerStateJump::PlayerStateJump(Player*owner):
	PlayerState::PlayerState(owner) {

}
void PlayerStateJump::update(float delta_time) {
	owner_->jump(delta_time);

}
void PlayerStateJump::start() {

}
void PlayerStateJump::end() {

}