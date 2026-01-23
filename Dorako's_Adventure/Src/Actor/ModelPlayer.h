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

};

#endif // !MODEL_PLAYER_H_
