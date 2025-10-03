#include "Kuribo.h"
#include "../../Assets.h"
#include "../../Collider/BoundingSphere.h"
#include "EnemyState/EnemyStateSearch.h"
#include <imgui/imgui.h>

Kuribo::Kuribo(IWorld* world, GSvector3 position) :
	Enemy{ Mesh_Kuribo } {
	world_ = world;
	name_ = "Kuribo";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere(1.0f, GSvector3{ 0.0f,enemy_height_,0.0f });
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());

	state_.add_state(EnemyState::Search, new EnemyStateSearch(this));
	state_.change_state(EnemyState::Search);
}
void Kuribo::update(float delta_time) {
	collide_field();
	collide_ground();
	gravity_update(delta_time);
	////メッシュのモーションを更新
	//mesh_->update(delta_time);
	////ワールド変換行列を設定
	//mesh_->transform(transform_.localToWorldMatrix());

	//デバック表示
	ImGui::Begin("Kuribo");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);

	ImGui::End();


}
void Kuribo::draw()const {
	mesh_->draw();
}
void Kuribo::react(Actor& other) {

}
void Kuribo::search(float delta_time) {
	velocity_ += transform_.forward() * walk_speed_ * delta_time;
	transform_.translate(velocity_, GStransform::Space::World);
}

