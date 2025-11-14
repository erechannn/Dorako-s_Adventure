#include "EnemyStateIdle.h"
#include "../EnemiesMotion.h"

EnemyStateIdle::EnemyStateIdle(Enemy* owner) :
	EnemyState{ owner } {

}
void EnemyStateIdle::update(float delta_time) {
	owner_->mesh_->change_motion(EnemiesMotion::Idle, true);
	owner_->idle(delta_time);
}
void EnemyStateIdle::start() {

}
void EnemyStateIdle::end() {

}