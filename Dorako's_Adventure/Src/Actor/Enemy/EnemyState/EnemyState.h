#ifndef ENEMY_STATE_H_
#define ENEMY_STATE_H_


#include "../../../State/IState.h"
#include"../Enemy.h"

class EnemyState :public IState {
public:
	EnemyState(Enemy* owner) :owner_{ owner } {};
	virtual void update(float delta_time)override {};
	virtual void start()override {};
	virtual void end()override {};
	enum State {
		Idle,
		Search,
		Chase,
		Attack,
		FirstFireAttack,
		Damage,
		Dead,
	};

protected:
	Enemy* owner_;
};









#endif // !ENEMY_STATE_H_



