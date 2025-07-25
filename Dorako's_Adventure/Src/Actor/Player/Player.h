#ifndef PLAYER_H_
#define PLAYER_H_

#include"../Actor.h"
#include"../../State/StateMachine.h"

class Player :public Actor {
public:
	Player(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);
	void move(float delta_time);
	void jump(float delta_time);
	void flying(float delta_time);
	void landing(float delta_time);
	void attack();
	void change_state(int next_state);
protected:
	GSvector2 move_input();
	StateMachine state_;
	void collide_field();
};



#endif // !PLAYER_H_
