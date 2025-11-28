#ifndef PLAYER_STATE_JUMP_H_
#define PLAYER_STATE_JUMP_H_

#include "PlayerState.h"

class PlayerStateJump :public PlayerState {
public:
	PlayerStateJump(Player* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
private:
	float jump_power_{ 0.3f };
};



#endif // !PLAYER_STATE_JUMP_H_
