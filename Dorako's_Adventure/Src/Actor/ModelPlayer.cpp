#include "ModelPlayer.h"
#include "Player/PlayerMotion.h"
#include "../World/IWorld.h"
#include "../Assets.h"

const float WalkSpeed{ 0.15f };

ModelPlayer::ModelPlayer(IWorld* world, GSvector3 position) :
	Character{ Mesh_Player } {
	world_ = world;
	name_ = "ModelPlayer";
	tag_ = "ModelTag";
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	mesh_->change_motion(PlayerMotion::Run,true);
}
void ModelPlayer::update(float delta_time) {
	mesh_->update(delta_time);
	//フィールドとの当たり判定
	collide_field();
	gravity_update(delta_time);
	//プレイヤーを地面に対して垂直に立たせる
	collide_ground();
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());

	velocity_ = {0.0f, 0.0f,  WalkSpeed * delta_time };


	transform_.translate(velocity_);
}
void ModelPlayer::draw()const {
	mesh_->draw();
}
void ModelPlayer::react(Actor& other) {

}