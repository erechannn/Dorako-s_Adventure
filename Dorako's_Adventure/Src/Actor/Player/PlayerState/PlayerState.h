#ifndef PLAYER_STATE_H_
#define PLAYER_STATE_H_

#include"../../../State/IState.h"
#include"../PlayerMotion.h"
#include"../Player.h"

class PlayerState :public IState {
public:
	PlayerState(Player* owner) :owner_{ owner } {};
	virtual void update(float delta_time)override {};
	virtual void start()override {};
	virtual void end()override {};

	enum State {
		StateMove,
		StateJumpStart,
		StateFlying,
		StateJumpEnd,
		StateAttack,
		StateDamage,
		StateDead,
		StateMotionOverride
	};

protected:
	Player* owner_;
};




#endif // !PLAYER_STATE_H_
