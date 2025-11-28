#ifndef BOSS_ATTACK_STATE_H_
#define BOSS_ATTACK_STATE_H_

#include "../../../State/IState.h"
#include "MiniDragon.h"

class BossAttackState :public IState {
public:
	BossAttackState(MiniDragon* owner) :owner_{ owner } {};
	virtual void update(float delta_time)override {};
	virtual void start()override {};
	virtual void end()override {};
	enum State {
		FirstFireAttack,
		FireAttack,
		RunAway,
		ChargeAttack,
	};
protected:
	MiniDragon* owner_;
};





#endif // !BOSS_ATTACK_STATE_H_
