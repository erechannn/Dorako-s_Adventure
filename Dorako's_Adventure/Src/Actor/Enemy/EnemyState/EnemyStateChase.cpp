#include "EnemyStateChase.h"
#include "../EnemiesMotion.h"

EnemyStateChase::EnemyStateChase(Enemy* owner) :
	EnemyState{ owner } {

}
void EnemyStateChase::update(float delta_time) {
	owner_->chase(delta_time);
	owner_->mesh_->change_motion(EnemiesMotion::Move);
}
void EnemyStateChase::start() {

}
void EnemyStateChase::end() {

}