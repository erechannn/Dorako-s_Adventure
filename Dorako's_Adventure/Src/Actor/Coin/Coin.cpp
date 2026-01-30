#include "Coin.h"
#include "../../World/IWorld.h"
#include "../../Assets.h"
#include "../../Stage/StageManager.h"

#include <GSeffect.h>


Coin::Coin(IWorld* world, GSvector3 position) :
	Character{ Mesh_Coin } {
	world_ = world;
	name_ = "Coin";
	tag_ = "CoinTag";
	collider_ = BoundingSphere{ 0.5f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	foot_offset_ = 0.01;
	mesh_->transform(transform_.localToWorldMatrix());
}
void Coin::update(float delta_time) {
	//コインの動きを書く
	mesh_->update(delta_time);
	//コイン地面に対して垂直に立たせる
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	GSvector3 up = transform_.position() - planet_position;
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	GSvector3 forward = GSvector3::cross(left, up);
	transform_.rotation(GSquaternion::lookRotation(forward, up));
	mesh_->transform(transform_.localToWorldMatrix());
}
void Coin::draw()const {
	mesh_->draw();
	//collider().draw();
}
void Coin::react(Actor& other) {
	if (other.name() == "Player") {
		world_->add_score(1);
		GSvector3 effect_position = transform_.position();
		effect_handle_ = gsPlayEffect(Effect_GetCoin, &effect_position);
		die();
	}
}