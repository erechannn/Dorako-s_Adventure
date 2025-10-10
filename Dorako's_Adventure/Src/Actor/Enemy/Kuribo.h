#ifndef KURIBO_H_
#define KURIBO_H_

#include "Enemy.h"

class Kuribo :public Enemy {
public:
	Kuribo(IWorld* world, GSvector3 position);
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other)override;
	virtual void search(float delta_time)override;
private:
	bool is_above_player(Actor& other);
	GSvector3 target_point_{ 0.0f,0.0f,0.0f };
	GSvector3 first_position_;
	float walk_timer_{ 0.0f };
	bool is_move_{ false };
	bool undead_{ false };
};



#endif // !KURIBO_H_
