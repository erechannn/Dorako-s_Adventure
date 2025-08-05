#include"World.h"
#include"Field.h"
#include"Actor/Actor.h"

World::~World() {
	clear();
}

void World::update(float delta_time) {
	field_->update(delta_time);
	actors_.update(delta_time);
	actors_.collide();
	actors_.late_update(delta_time);
	actors_.remove();
	camera_->update(delta_time);
	light_->update(delta_time);
}

void World::draw()const {
	camera_->draw();
	light_->draw();
	gsDrawShadowMap(World::shadow_map_callback, (void*)this);

	field_->draw();
	actors_.draw();
	actors_.draw_transparent();
	actors_.draw_gui();
}
void World::clear() {
	actors_.clear();
	delete camera_;
	camera_ = nullptr;
	delete light_;
	light_ = nullptr;
	delete field_;
	field_ = nullptr;
}

void World::add_camera(Actor* camera) {
	delete camera_;
	camera_ = camera;
}
void World::add_light(Actor* light) {
	delete light_;
	light_ = light;
}
void World::add_field(Field* field) {
	delete field_;
	field_ = field;
}

void World::add_actor(Actor* actor) {
	actors_.add(actor);
}
Actor* World::find_actor(const std::string& name)const {
	return actors_.find(name);
}
std::vector<Actor*>World::find_actor_with_tag(const std::string& tag)const {
	return actors_.find_with_tag(tag);
}
int World::count_actor()const {
	return actors_.count();
}
int World::count_actor_with_tag(const std::string& tag)const {
	return actors_.count_with_tag(tag);
}
void World::send_message(const std::string& message, void* param) {
	actors_.send_message(message, param);
}

Actor* World::camera() {
	return camera_;
}

Actor* World::light() {
	return light_;
}

Field* World::field() {
	return field_;
}
void World::shadow_map_callback(void* param, const GSmatrix4*, const GSmatrix4*) {
	World* self = (World*)param;
	// シャドウマップにはアクターのみ描画
	self->actors_.draw();
}
