#include "WoodBox.h"
#include "../../World/IWorld.h"
#include"../../Assets.h"

#include <imgui/imgui.h>

WoodBox::WoodBox(IWorld* world, GSvector3 position) :
	Character{ Mesh_WoodBox } {
	world_ = world;
	name_ = "WoodBox";
	tag_ = "WoodTag";
	collider_ = BoundingSphere{ 1.0f,{0.0f,0.5f,0.0f} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	foot_offset_ = 5.0f;

}

void WoodBox::update(float delta_time) {
	mesh_->update(delta_time);
	gravity_update(delta_time); 
	collide_field();
	collide_ground();
	mesh_->transform(transform_.localToWorldMatrix());
	collider_.transform(transform_.localToWorldMatrix());

	//デバック表示
	ImGui::Begin("WoodBox");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("gravity:%f", gravity_);
	ImGui::Checkbox("地上にいるか", &is_ground_);

	ImGui::End();
}
void WoodBox::draw()const {
	mesh_->draw();
	collider().draw();
}
void WoodBox::react(Actor& other) {
	collide_actor(other);
}
bool WoodBox::is_above_actor(Actor& other) {
	GSvector3 position = transform_.position();
	GSvector3 other_position = other.transform().position();
	GSvector3 up = transform_.up();
	GSvector3 to_target = other_position - position;
	float dot = to_target.dot(up);
	return dot > 0;

}
