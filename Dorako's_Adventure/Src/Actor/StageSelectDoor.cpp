#include "StageSelectDoor.h"
#include "../World/IWorld.h"
#include "../Assets.h"
#include "../Stage/StageManager.h"

#include <GSeffect.h>

enum {
	Locked_Door=0,
	Open_Door=1
};


StageSelectDoor::StageSelectDoor(IWorld* world, GSvector3 position, int stage_id) :
	Actor{ 0 } {
	world_ = world;
	tag_ = "DoorTag";
	name_ = "StageSelectDoor";
	collider_ = BoundingSphere{ 1.0f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	stage_id_ = stage_id;
	is_unlock_ = StageManager::get_instance().is_stage_unlocked(stage_id_);
	if (is_unlock_) {
		//空いているドアのエフェクト
		effect_handle_ = gsPlayEffect(Effect_Teleportation, &position);
	}
	
}
void StageSelectDoor::update(float delta_time) {
	GSmatrix4 world = transform_.localToWorldMatrix();
	gsSetEffectMatrix(effect_handle_, &world); // ワールド変換行列を設定
	Actor* player = world_->find_actor("Player");
	//プレイヤーが当たっていなければ入るフラグを消す
	if (!is_collide(*player)) {
		is_into_the_door_ = false;
	}
}
void StageSelectDoor::draw()const {
}
void StageSelectDoor::react(Actor& other) {
	//プレイヤーが当たったら入るフラグを点ける
	if (other.name() == "Player") {
		is_into_the_door_ = true;
	}
}
//自身のステージIDを返す
int StageSelectDoor::get_stage_id() {
	return stage_id_;
}
//入れるかのフラグを返す
bool StageSelectDoor::is_into_the_door() {
	return is_into_the_door_;
}