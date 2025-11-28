#ifndef ATTACK_STATE_FIRST_FIRE_ATTACK_H_
#define ATTACK_STATE_FIRST_FIRE_ATTACK_H_

#include "../EnemyState/EnemyState.h"

class AttackStateFirstFireAttack :public EnemyState {
public:
	AttackStateFirstFireAttack(Enemy* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};








#endif // !ATTACK_STATE_FIRST_FIRE_ATTACK_H_
