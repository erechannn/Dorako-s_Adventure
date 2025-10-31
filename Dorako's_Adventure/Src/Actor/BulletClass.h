#ifndef BULLET_CLASS_H_
#define BULLET_CLASS_H_

#include "Character.h"
#include <string>

class BulletClass :public Character {
public:
	BulletClass(IWorld* world, const GSvector3& position, const GSvector3& velocity,GSint effect_handle,
		const std::string& tag,const std::string& name,const std::string& owner_tag="");
	virtual void update(float delta_time)override;
	virtual void draw()const override;
	virtual void react(Actor& other)override;
private:
	float lifespan_timer_{ 300.0f };
	std::string owner_tag_{ " " };
	GSvector3 effect_position_{ 0.0f,0.0f,0.0f };
	GSint effect_handle_{ 0 };
	float speed_{ 0.1f };

};





#endif // !BULLET_CLASS_H_
