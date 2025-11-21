#include "EnemyStateDead.h"
#include "../EnemiesMotion.h"

EnemyStateDead::EnemyStateDead(Enemy* owner) :
	EnemyState{owner} {

}
void EnemyStateDead::update(float delta_time) {
	owner_->mesh_->change_motion(EnemiesMotion::Dead, false);
	if (owner_->mesh_->is_end_motion()) {
		owner_->die();
	}
}
void EnemyStateDead::start() {

}
void EnemyStateDead::end() {

}