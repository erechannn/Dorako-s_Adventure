#ifndef PLAYER_STATE_DAMAGE_H_
#define PLAYER_STATE_DAMAGE_H_

#include"PlayerState.h"

class PlayerStateDamage :public PlayerState {
public:
	PlayerStateDamage(Player* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;

};
#endif // !PLAYER_STATE_DAMAGE_H_
