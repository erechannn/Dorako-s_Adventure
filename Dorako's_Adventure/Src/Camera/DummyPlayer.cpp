#include"DummyPlayer.h"
#include"../World/World.h"

#include <iostream>
#include <imgui/imgui.h>

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

	ImGui::Begin("DummyPlayer");
	ImGui::Text("position:x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("up:x:%f y:%f z:%f", transform_.up().x, transform_.up().y, transform_.up().z);
	ImGui::Text("forward:x:%f y:%f z:%f", transform_.forward().x, transform_.forward().y, transform_.forward().z);
	ImGui::End();

}