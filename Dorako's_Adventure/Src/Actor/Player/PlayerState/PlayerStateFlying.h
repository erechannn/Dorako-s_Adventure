#ifndef PLAYER_STATE_FLYING_H_
#define PLAYER_STATE_FLYING_H_

#include "PlayerState.h"

class PlayerStateFlying :public PlayerState {
public:
	PlayerStateFlying(Player* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};







#endif // !PLAYER_STATE_FLYING_H_
