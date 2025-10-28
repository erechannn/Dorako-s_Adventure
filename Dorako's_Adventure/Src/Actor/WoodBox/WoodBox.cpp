#include "WoodBox.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include"../../Assets.h"

#include <imgui/imgui.h>

const float FootOffset_{ 0.5f };

WoodBox::WoodBox(IWorld* world, GSvector3 position,GSuint mesh,GSuint collider) {
	world_ = world;
	name_ = "WoodBox";
	tag_ = "WoodTag";
	collider_ = BoundingSphere{ 1.0f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	mesh_ = mesh;
	mesh_collider_ = collider;
}

void WoodBox::update(float delta_time) {
	gravity_update(delta_time); 
	//collide_field();
	collide_ground();
	collider_.transform(transform_.localToWorldMatrix());

	//デバック表示
	ImGui::Begin("WoodBox");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);

	ImGui::End();
}
void WoodBox::draw()const {
	collider().draw();
}
void WoodBox::react(Actor& other) {
	if (!is_above_actor(other)) {
		collide_actor(other);
	}
}
void WoodBox::collide_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 押し戻し後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// 補正後の座標に変更する
		transform_.position(center);
	}

}
bool WoodBox::is_above_actor(Actor& other) {
	GSvector3 position = transform_.position();
	GSvector3 other_position = other.transform().position();
	GSvector3 up = transform_.up();
	GSvector3 to_target = other_position - position;
	float dot = to_target.dot(up);
	return dot > 0;
}
