#ifndef PLAYER_STATE_MOVE_H_
#define PLAYER_STATE_MOVE_H_

#include"PlayerState.h"
class PlayerStateMove :public PlayerState {
public:
	PlayerStateMove(Player* owner);
	void update(float delta_time)override;
	void start()override;
	void end()override;
};

#endif // !PLAYER_STATE_MOVE_H_
