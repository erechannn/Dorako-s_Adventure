#include "MainLight.h"
#include "../World/IWorld.h"

Light::Light(IWorld* world) {
	world_ = world;
	name_ = "Light";
	tag_ = "LightTag";
}
void Light::update(float delta_time) {
	camera_ = world_->camera();
	if(camera_!=nullptr)
	camera_position_ = camera_->transform().position();
}

void Light::draw()const {
	float ambient[4] = { 0.3f,0.3f,0.3f,1.0f };
	float diffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
	float specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float position[4] = { camera_position_.x,camera_position_.y,camera_position_.z,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);

}