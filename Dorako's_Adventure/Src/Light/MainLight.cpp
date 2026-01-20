#include "MainLight.h"


Light::Light(IWorld* world) {
	world_ = world;
	name_ = "Light";
	tag_ = "LightTag";
}

void Light::draw()const {
	float ambient[4] = { 0.2f,0.2f,0.2f,1.0f };
	float diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	float specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float position[4] = { 100.0f,100.0f,100.0f,0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);

}