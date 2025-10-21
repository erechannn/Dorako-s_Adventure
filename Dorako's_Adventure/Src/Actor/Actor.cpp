#include "Actor.h"


Actor::Actor(GSuint mesh)
{
	mesh_ = { new AnimatedMesh(mesh) };
}

Actor::~Actor()
{
	delete mesh_;
	mesh_ = nullptr;
}


void Actor::update(float) {}

void Actor::late_update(float) {}

void Actor::draw()const {}

void Actor::draw_transparent()const {}

void Actor::draw_gui()const {}

void Actor::react(Actor&) {}

void Actor::handle_message(const std::string& message, void* param) {}

void Actor::collide(Actor& other) {
	if (enable_collider_ && other.enable_collider_) {
		if (is_collide(other)) {
			react(other);
			other.react(*this);
		}
	}
}

void Actor::die() {
	dead_ = true;
}

TweenUnit& Actor::move_to(const GSvector3& to, float duration) {
	// Œ»İ‚ÌêŠ‚©‚çw’è‚³‚ê‚½êŠ‚Ü‚ÅATween‚ÅˆÚ“®‚·‚é
	return Tween::vector3(transform_.position(), to, duration,
		[=](GSvector3 pos) {transform_.position(pos); });
}


bool Actor::is_collide(const Actor& other)const {
	return collider().intersects(other.collider());
}

bool Actor::is_dead()const {
	return dead_;
}

const std::string& Actor::name()const {
	return name_;
}

const std::string& Actor::tag()const {
	return tag_;
}

const GStransform& Actor::transform()const {
	return transform_;
}

GStransform& Actor::transform() {
	return transform_;
}

GSvector3 Actor::velocity()const {
	return velocity_;
}

BoundingSphere Actor::collider()const {
	return collider_.transform(transform_.localToWorldMatrix());
}