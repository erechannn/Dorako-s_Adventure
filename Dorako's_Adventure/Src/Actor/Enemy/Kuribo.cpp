#include "Kuribo.h"
#include "../../Assets.h"
#include "../../Collider/BoundingSphere.h"
#include "EnemyState/EnemyStateSearch.h"
#include <imgui/imgui.h>
#include <algorithm>

const float M_PI{ 3.14159265358979323846 };

Kuribo::Kuribo(IWorld* world, GSvector3 position) :
	Enemy{ Mesh_Kuribo } {
	world_ = world;
	name_ = "Kuribo";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere(1.0f, GSvector3{ 0.0f,height_,0.0f });
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	walk_speed_ = 0.5f;
	foot_offset_ = 5.0f;
	state_.add_state(EnemyState::Search, new EnemyStateSearch(this));
	state_.change_state(EnemyState::Search);
}
void Kuribo::update(float delta_time) {
	state_.update(delta_time);
	collide_field();
	collide_ground();
	gravity_update(delta_time);
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());

	//デバック表示
	ImGui::Begin("Kuribo");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("x:%f y:%f z:%f", target_point_.x, target_point_.y, target_point_.z);
	ImGui::Checkbox("is_move:",&is_move_);
	ImGui::Checkbox("undead:",&undead_);
	ImGui::End();
}
void Kuribo::draw()const {
	mesh_->draw();
}
void Kuribo::react(Actor& other) {
	if (is_above_player(other)&&!undead_&&other.tag()=="PlayerTag") {
		die();
	}
}
void Kuribo::search(float delta_time) {

	//とりあえずまっすぐに移動
	if (is_move_) {
		transform_.translate(0.0f, 0.0f, walk_speed_ * delta_time);
	}
}
bool Kuribo::is_above_player(Actor& other) {
	GSvector3 position = transform_.position();
	GSvector3 other_position = other.transform().position();
	GSvector3 up = transform_.up();
	GSvector3 to_target = other_position - position;
	float dot = to_target.dot(up);
	return dot > 0;
}

/*
void Kuribo::angle_set(GSvector3 target, GSvector3 forward) {
	// ターゲット方向のベクトルを求める
	GSvector3 to_target = target - transform_.position();

	// ベクトルのy成分を無効にする
	forward.y = 0.0f;
	to_target.y = 0.0f;

	// 方向を正規化
	forward = GSvector3::normalize(forward);
	to_target = GSvector3::normalize(to_target);

	// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号付き）
	float target_angle = GSvector3::signedAngle(forward, to_target);

	const float max_rotation_speed = 1.5f; // 毎フレームの最大回転量

	// 回転角度を制限する
	float clamped_angle = std::clamp(target_angle, -max_rotation_speed, max_rotation_speed);

	// 回転を加える
	transform_.rotate(0.0f, clamped_angle, 0.0f);

}
void Kuribo::set_next_point() {
	GSvector3 center = first_position_;
	GSvector3 target_;
	float radius = 2.0f;

	// ランダムな角度（0～2π）
	float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;

	// ランダムな距離（0.0～radius）
	float dist = static_cast<float>(std::rand()) / RAND_MAX * radius;

	// ターゲット座標を設定
	target_.x = center.x + std::cos(angle) * radius;
	target_.y = 0.0f;
	target_.z = center.z + std::sin(angle) * radius;

	transform_.transformDirection(target_);

	target_point_ = target_;

}
*/
