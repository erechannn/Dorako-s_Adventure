#ifndef ENEMY_STATE_CHASE_H_
#define ENEMY_STATE_CHASE_H_

#include "EnemyState.h"

class EnemyStateChase :public EnemyState {
public:
	EnemyStateChase(Enemy* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};






#endif // !ENEMY_STATE_CHASE_H_
