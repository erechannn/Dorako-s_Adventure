#include "Coin.h"
#include "../../World/IWorld.h"
#include "../../Assets.h"
#include "../../Stage/StageManager.h"


Coin::Coin(IWorld* world, GSvector3 position) :
	Character{ Mesh_Coin } {
	world_ = world;
	name_ = "Coin";
	tag_ = "CoinTag";
	collider_ = BoundingSphere{ 1.0f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	foot_offset_ = 0.01;
	mesh_->transform(transform_.localToWorldMatrix());
}
void Coin::update(float delta_time) {
	//コインの動きを書く
	mesh_->update(delta_time);
	//コイン地面に対して垂直に立たせる
	collide_ground();
	mesh_->transform(transform_.localToWorldMatrix());
}
void Coin::draw()const {
	mesh_->draw();
}
void Coin::react(Actor& other) {
	if (other.name() == "Player") {
		world_->add_score(1);
		die();
	}
}