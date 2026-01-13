#include "PlayerStateAttack.h"
#include "../../BulletClass.h"
#include "../../../Assets.h"
#include "../../../World/IWorld.h"

PlayerStateAttack::PlayerStateAttack(Player* owner,IWorld* world) :
	PlayerState{owner}
{
	world_ = world;
}
void PlayerStateAttack::update(float delta_time) {
	owner_->mesh_->change_motion(PlayerMotion::Attack, false);
	if (owner_->is_motion_end()) {
		owner_->change_state(PlayerState::StateMove);
	}
}
void PlayerStateAttack::start() {
	if (owner_->fire_count() != 0) {
		GSvector3 position = owner_->transform().position() + owner_->transform().forward();
		world_->add_actor(new BulletClass{ world_,position,owner_->transform().forward(),Effect_FireBoll,"PlayerAttackTag","PlayerFireAttack",owner_->tag() });
		owner_->consume_fire_count();
	}
}
void PlayerStateAttack::end() {

}