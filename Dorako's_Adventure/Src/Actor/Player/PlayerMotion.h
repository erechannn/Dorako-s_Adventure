#ifndef PLAYER_MOTION_H_
#define PLAYER_MOTION_H_

class PlayerMotion {
public:
	enum Motion{
		Idle=0,
		Walk,
		Run,
		JumpStart,
		Flying,
		JumpEnd,
		Attack,
		Damage,
		Die,
	};
};




#endif // !PLAYER_MOTION_H_
