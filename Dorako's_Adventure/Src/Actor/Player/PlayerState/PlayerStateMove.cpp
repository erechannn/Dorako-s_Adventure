#include"PlayerStateMove.h"

PlayerStateMove::PlayerStateMove(Player*owner):
	PlayerState::PlayerState(owner) {

}

void PlayerStateMove::update(float delta_time) {
	owner_->move(delta_time);
	if (!owner_->is_ground()) {
		owner_->change_state(PlayerState::StateFlying);
	}
}
void PlayerStateMove::start() {

}
void PlayerStateMove::end() {

}