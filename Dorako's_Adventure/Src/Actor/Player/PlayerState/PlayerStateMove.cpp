#include"PlayerStateMove.h"

PlayerStateMove::PlayerStateMove(Player*owner):
	PlayerState::PlayerState(owner) {

}

void PlayerStateMove::update(float delta_time) {
	owner_->move(delta_time);
}
void PlayerStateMove::start() {

}
void PlayerStateMove::end() {

}