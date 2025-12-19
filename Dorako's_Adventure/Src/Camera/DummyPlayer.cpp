#include"DummyPlayer.h"
#include"../World/World.h"

#include <iostream>

DummyPlayer::DummyPlayer(IWorld*world) {
	world_ = world;
	name_ = "DummyPlayer";
	tag_ = "DummyPlayerTag";
	
}
void DummyPlayer::update(float delta_time) {
	player_ = world_->find_actor("Player");
	if (player_ == nullptr)return;
	transform_.position(player_->transform().position());
	//transform_.up(player_->transform().up());
	GSvector3 up = player_->transform().up();
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	GSvector3 forward = GSvector3::cross(left,up);
	transform_.rotation(GSquaternion::lookRotation(forward, up));
	//transform_.forward(world_->camera()->transform().forward());
	//transform_.right(world_->camera()->transform().right());
}