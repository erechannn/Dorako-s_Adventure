#include "AttackStateFirstFireAttack.h"

AttackStateFirstFireAttack::AttackStateFirstFireAttack(Enemy* owner) :
	EnemyState{ owner } {

}
void AttackStateFirstFireAttack::update(float delta_time) {
	owner_->first_fire_attack(delta_time);
}
void AttackStateFirstFireAttack::start() {

}
void AttackStateFirstFireAttack::end() {

}
