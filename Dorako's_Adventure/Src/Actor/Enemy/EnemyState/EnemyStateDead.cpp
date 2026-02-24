#include "EnemyStateDead.h"
#include "../EnemiesMotion.h"
#include "../../Item/ItemState.h"

EnemyStateDead::EnemyStateDead(Enemy* owner) :
	EnemyState{owner} {

}
void EnemyStateDead::update(float delta_time) {
	owner_->dead(delta_time);
	owner_->mesh_->change_motion(EnemiesMotion::Dead, false);
	if (owner_->mesh_->is_end_motion()) {
		if (owner_->name() == "Kuribo") {
			owner_->spawn_item(Item::Fire);
		}
		if (owner_->tag() == "BossEnemyTag") {
			owner_->spawn_item(Item::CoinItem);
		}
		owner_->die();
	}
}
void EnemyStateDead::start() {

}
void EnemyStateDead::end() {

}