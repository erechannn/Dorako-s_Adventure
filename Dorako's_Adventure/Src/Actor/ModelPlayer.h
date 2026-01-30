#ifndef MODEL_PLAYER_H_
#define MODEL_PLAYER_H_

#include "Character.h"

//タイトルにいるプレイヤー
class ModelPlayer :public Character {
public:
	ModelPlayer(IWorld* world, GSvector3 position);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other);
private:
	void to_target(GSvector3 target, float delta_time);
private: 
	GSvector3 target_point_{};
	GSvector3 target_points_[2];

};

#endif // !MODEL_PLAYER_H_
