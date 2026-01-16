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
	Actor{ Mesh_Door } {
	world_ = world;
	tag_ = "DoorTag";
	name_ = "StageSelectDoor";
	collider_ = BoundingSphere{ 1.0f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	stage_id_ = stage_id;
	is_unlock_ = StageManager::get_instance().is_stage_unlocked(stage_id_);
	effect_handle_ = gsPlayEffect(Effect_Teleportation, &position);
	
}
void StageSelectDoor::update(float delta_time) {
	GSmatrix4 world = transform_.localToWorldMatrix();
	gsSetEffectMatrix(effect_handle_, &world); // ƒ[ƒ‹ƒh•ÏŠ·s—ñ‚ðÝ’è
	Actor* player = world_->find_actor("Player");
	if (is_unlock_) {
		mesh_->change_motion(Open_Door, false);
	}
	if (!is_collide(*player)) {
		is_into_the_door_ = false;
	}
}
void StageSelectDoor::draw()const {
	mesh_->draw();
}
void StageSelectDoor::react(Actor& other) {
	if (other.name() == "Player") {
		is_into_the_door_ = true;
	}
}

int StageSelectDoor::get_stage_id() {
	return stage_id_;
}
bool StageSelectDoor::is_into_the_door() {
	return is_into_the_door_;
}