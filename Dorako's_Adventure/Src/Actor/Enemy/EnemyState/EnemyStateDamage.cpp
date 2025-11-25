#include "EnemyStateDamage.h"
#include "../EnemiesMotion.h"

EnemyStateDamage::EnemyStateDamage(Enemy* owner) :
	EnemyState{ owner } {

}
void EnemyStateDamage::update(float delta_time) {
	owner_->damage(delta_time);
	owner_->mesh_->change_motion(EnemiesMotion::Damage);
	if (owner_->mesh_->is_end_motion()) {
		owner_->change_state(EnemyState::Idle);
	}
}