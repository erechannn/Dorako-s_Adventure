#ifndef ENEMY_STATE_DAMAGE_H_
#define ENEMY_STATE_DAMAGE_H_

#include "EnemyState.h"

class EnemyStateDamage :public EnemyState {
public:
	EnemyStateDamage(Enemy* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};






#endif // !ENEMY_STATE_DAMAGE_H_
