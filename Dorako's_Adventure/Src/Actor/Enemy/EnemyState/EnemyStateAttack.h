#ifndef ENEMY_STATE_ATTACK_H_
#define ENEMY_STATE_ATTACK_H_

#include "EnemyState.h"

class EnemyStateAttack :public EnemyState {
public:
	EnemyStateAttack(Enemy* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};




#endif // !ENEMY_STATE_ATTACK_H_
