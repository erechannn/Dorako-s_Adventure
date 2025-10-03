#ifndef ENEMY_STATE_SEARCH_H_
#define ENEMY_STATE_SEARCH_H_

#include "EnemyState.h"

class EnemyStateSearch :public EnemyState {
public:
	EnemyStateSearch(Enemy* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};




#endif // !ENEMY_STATE_SEARCH_H_
