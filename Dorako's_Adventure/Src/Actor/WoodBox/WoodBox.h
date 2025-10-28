#ifndef WOOD_BOX_H_
#define WOOD_BOX_H_

#include "../Character.h"
#include "../FieldActor/FieldActor.h"

class WoodBox :public FieldActor {
public:
	WoodBox(IWorld* world, GSvector3 position,GSuint mesh,GSuint mesh_collider);
	void update(float delta_time)override;
	void draw()const override;
	void react(Actor& other)override;
private:
	void collide_field();
	bool is_above_actor(Actor& other);
};


#endif // !WOOD_BOX_H_
