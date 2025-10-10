#ifndef PLAYER_STATE_DEAD_H_
#define PLAYER_STATE_DEAD_H_

#include"PlayerState.h"

class PlayerStateDead :public PlayerState {
public:
	PlayerStateDead(Player* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};




#endif // !PLAYER_STATE_DEAD_H_
