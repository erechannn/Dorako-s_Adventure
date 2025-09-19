#include"DummyPlayer.h"
#include"../World/World.h"


DummyPlayer::DummyPlayer(IWorld*world) {
	world_ = world;
	name_ = "DummyPlayer";
	tag_ = "DummyPlayerTag";
	
}
void DummyPlayer::update(float delta_time) {
	player_ = world_->find_actor("Player");
	if (player_ == nullptr)return;
	transform_.position(player_->transform().position());
	transform_.up(player_->transform().up());
	//transform_.forward(world_->camera()->transform().forward());
	//transform_.right(world_->camera()->transform().right());
}