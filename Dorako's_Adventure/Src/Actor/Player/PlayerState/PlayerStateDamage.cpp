#include"PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage(Player* owner)
	:PlayerState::PlayerState(owner) {

}
void PlayerStateDamage::update(float delta_time) {
	owner_->jump(delta_time);
}
void PlayerStateDamage::start() {

}
void PlayerStateDamage::end() {

}