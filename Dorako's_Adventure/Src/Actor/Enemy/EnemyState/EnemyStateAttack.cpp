#include "EnemyStateAttack.h"
#include "../EnemiesMotion.h"

EnemyStateAttack::EnemyStateAttack(Enemy* owner) :
	EnemyState{ owner } {

}
void EnemyStateAttack::update(float delta_time) {
	owner_->attack(delta_time);
}
void EnemyStateAttack::start() {

}
void EnemyStateAttack::end() {

}
