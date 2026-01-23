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
	//場所をプレイヤーに合わせて更新
	transform_.position(player_->transform().position());
	//上方向ベクトルをプレイヤーに合わせて更新
	GSvector3 up = player_->transform().up();
	//上方向ベクトルと現在の正面ベクトルから左方向ベクトルを求める
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	//新たな正面ベクトルを計算
	GSvector3 forward = GSvector3::cross(left,up);
	//正面ベクトルをを更新
	transform_.rotation(GSquaternion::lookRotation(forward, up));
}