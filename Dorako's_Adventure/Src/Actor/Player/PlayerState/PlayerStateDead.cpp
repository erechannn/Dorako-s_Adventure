#include"PlayerStateDead.h"

PlayerStateDead::PlayerStateDead(Player* owner) :
	PlayerState(owner) {

}
void PlayerStateDead::update(float delta_time) {
	
	owner_->mesh_->change_motion(PlayerMotion::Die,false);
	if (owner_->is_motion_end()) {
		owner_->die();
	}
}
void PlayerStateDead::start() {

}
void PlayerStateDead::end() {

}