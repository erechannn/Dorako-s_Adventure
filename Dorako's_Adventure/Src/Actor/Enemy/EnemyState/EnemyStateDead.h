#ifndef ENEMY_STATE_DEAD_H_
#define ENEMY_STATE_DEAD_H_

#include "EnemyState.h"

class EnemyStateDead :public EnemyState {
public:
	EnemyStateDead(Enemy* owner);
	virtual void update(float delta_time)override;
	virtual void start()override;
	virtual void end()override;
};





#endif // !ENEMY_STATE_DEAD_H_
