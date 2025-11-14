#ifndef ENEMY_STATE_IDLE_H_
#define ENEMY_STATE_IDLE_H_


#include "EnemyState.h"

class EnemyStateIdle :public EnemyState {
public:
	EnemyStateIdle(Enemy* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};




#endif // !ENEMY_STATE_IDLE_H_
