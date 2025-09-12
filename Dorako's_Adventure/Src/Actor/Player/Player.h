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
	StateMachine state_;
	void collide_field();
	void collide_actor(Actor& other);
private:
	void player_rotate(float delta_time);
private:
	bool is_move_{ false };
	float gravity_{ -0.003f };
	float gravityXZ_{ -0.3f };
};



#endif // !PLAYER_H_
