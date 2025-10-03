#include "EnemyStateSearch.h"
#include "../EnemiesMotion.h"

EnemyStateSearch::EnemyStateSearch(Enemy* owner) :
	EnemyState{ owner } {

}
void EnemyStateSearch::update(float delta_time) {
	owner_->search(delta_time);
	owner_->mesh_->change_motion(EnemiesMotion::Move);
}
void EnemyStateSearch::start() {

}
void EnemyStateSearch::end() {

}