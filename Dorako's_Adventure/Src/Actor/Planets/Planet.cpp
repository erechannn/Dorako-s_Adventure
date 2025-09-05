#include"Planet.h"
#include"Assets.h"
#include"World/IWorld.h"
#include"World/Field.h"
#include"../../Shape/Line.h"

Planet::Planet(IWorld* world, GSvector3 position) :
	Actor{ Mesh_Planet } {
	world_ = world;
	name_ = "Planet";
	tag_ = "PlanetTag";
	collider_ = BoundingSphere{ 20,GSvector3{0,0,0} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
}
void Planet::update(float delta_time) {

}
void Planet::draw()const {
	mesh_->draw();
}
void Planet::react(Actor& other) {

}