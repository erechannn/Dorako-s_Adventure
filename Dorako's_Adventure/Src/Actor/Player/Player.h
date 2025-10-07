#ifndef PLAYER_H_
#define PLAYER_H_

#include"../Character.h"
#include"../../State/StateMachine.h"

class Player :public Character {
public:
	Player(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);
	void move(float delta_time);
	void flying(float delta_time);
	void landing(float delta_time);
	void attack();
//ステートマシンに送る数値
public:
	//モーションの終わり
	bool is_motion_end();
	//現在の体力
	int now_health_(){}
private:
	bool is_move_{ false };
	int max_health_{ 3 };
	int health_{ 3 };
	friend class PlayerState;
};



#endif // !PLAYER_H_
