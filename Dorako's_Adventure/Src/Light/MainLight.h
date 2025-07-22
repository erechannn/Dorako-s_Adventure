#ifndef LIGHT_H_
#define LIGHT_H_

#include "Actor/Actor.h"

class Light :public Actor {
public:
	Light(IWorld* world);
	virtual void draw()const override;

};


#endif // !LIGHT_H_H
