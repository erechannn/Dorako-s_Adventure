#ifndef PLAYER_STATE_ATTACK_H_
#define PLAYER_STATE_ATTACK_H_

#include"PlayerState.h"

class PlayerStateAttack :public PlayerState {
public:
	PlayerStateAttack(Player* owner,IWorld* world);
	void update(float delta_time)override;
	void start()override;
	void end()override;
private:
	IWorld* world_{ nullptr };
};






#endif // !PLAYER_STATE_ATTACK_H_
