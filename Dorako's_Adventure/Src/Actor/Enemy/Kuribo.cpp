#include "Kuribo.h"
#include "../../Assets.h"
#include "../../Collider/BoundingSphere.h"
#include "EnemyState/EnemyStateSearch.h"
#include <imgui/imgui.h>
#include <algorithm>
#include <iostream>

const float M_PI{ 3.14159265358979323846 };

Kuribo::Kuribo(IWorld* world, GSvector3 position) :
	Enemy{ Mesh_Kuribo } {
	world_ = world;
	name_ = "Kuribo";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere(1.0f, GSvector3{ 0.0f,height_,0.0f });
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	foot_offset_ = 2.0f;
	first_position_ = transform_.inverseTransformPoint(position);
	first_forward_ = transform_.forward();
	first_right_ = transform_.right();
	first_transform_ = transform_;
	set_next_point();
	state_.add_state(EnemyState::Search, new EnemyStateSearch(this));
	state_.change_state(EnemyState::Search);
}
void Kuribo::update(float delta_time) {
	state_.update(delta_time);
	std::cout << "x: " << transform_.position().x << " y: " << transform_.position().y << " z: " << transform_.position().z << std::endl;
	collide_field();
	gravity_update(delta_time);
	collide_ground();
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());

	test2_ = BoundingSphere(1.0f, transform_.position());

	bool next_point = false;

	float dis = GSvector3::distance(target_point_, transform_.position());
	if (!is_ground_) {
		std::cout << "異常発生" << std::endl;
	}

	//デバック表示
	ImGui::Begin("Kuribo");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("x:%f y:%f z:%f", target_point_.x, target_point_.y, target_point_.z);
	ImGui::Text("distance%f",dis);
	ImGui::Checkbox("is_move:",&is_move_);
	ImGui::Checkbox("undead:",&undead_);
	ImGui::Checkbox("is_ground", &is_ground_);
	ImGui::Checkbox("next: ", &next_point);
	ImGui::End();

	if (next_point) {
		set_next_point();
		next_point = false;

	}

}
void Kuribo::draw()const {
	mesh_->draw();
	test_.draw();
	test2_.draw();
	GSvector3 up = transform_.up();
	//std::cout << "2 x: " << collider_point_.x << " y: " << collider_point_.y << " z: " << collider_point_.z << std::endl;


}
void Kuribo::react(Actor& other) {
	if (is_above_player(other)&&!undead_&&other.tag()=="PlayerTag") {
		die();
	}
}
void Kuribo::search(float delta_time) {

	//とりあえずまっすぐに移動
	if (is_move_) {
		float dis = GSvector3::distance(target_point_, transform_.position());
		if (dis <= 0.3f) {
			set_next_point();
		}
		GSvector3 to_target = target_point_ - transform_.position();

		GSvector3 velocity = { 0.0f,0.0f,0.0f };

		float angle = GSvector3::angle(to_target, transform_.forward());
		if (angle <= 0.1f) {

		}
		GSvector3 target_normal = to_target.normalize();
		velocity = target_normal * walk_speed_ * delta_time;

		GSvector3 position = transform_.position();

		velocity_ = velocity;
		transform_.translate(velocity, GStransform::Space::World);

		GSvector3 planet_position = { 0.0f,-20.0f,0.0f };
		GSvector3 to_planet = transform_.position() - planet_position;
		to_planet.normalize();
		GSvector3 up = to_planet;
		//transform_.up(to_planet);
		to_planet = to_planet * 20.0f;
		to_planet = to_planet + planet_position;
		GSvector3 to_position = to_planet-position;
		to_position.normalize();
		GSvector3 left = GSvector3::cross(up, to_position);
		GSvector3 forward = GSvector3::cross(left, up);

		transform_.position(to_planet);
		transform_.rotation(GSquaternion::lookRotation(forward, up));
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

*/
void Kuribo::set_next_point() {
	GSvector3 center = first_transform_.inverseTransformPoint(first_transform_.position());
	GSvector3 forward = transform_.forward();
	GSvector3 right = transform_.right();
	GSvector3 target;
	float radius = 3.0f;

	// ランダムな角度（0～2π）
	float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;

	// ランダムな距離（0.0～radius）
	float dist = static_cast<float>(std::rand()) / RAND_MAX * radius;

	// ターゲット座標を設定
	// ターゲット座標を設定
	target.x = center.x + std::cos(angle) * radius;
	target.z = center.z + std::sin(angle) * radius;
	//GSvector3 offset = first_right_ * (std::cos(angle) * dist) + first_forward_ * (std::sin(angle) * dist);
	//target = center + offset;
	target = first_transform_.transformPoint(target);

	GSvector3 planet_position = { 0.0f,-20.0f,0.0f };
	target = target - planet_position;
	target = target.normalize();
	target = target * 20.0f;
	target = target + planet_position;

	test_ = BoundingSphere{ 1.0f,target };
	target_point_ = target;
	/*
	 vector3 center = first_position_;
    vector3 target;
    float radius = 2.0f;
    
    // ランダムな角度（0～2π）
    float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;
    // ランダムな距離（0.0～radius）
    float dist = static_cast<float>(std::rand()) / RAND_MAX * radius;
    
    // forwardとrightベクトルを使って接平面上の座標を計算
    // cos(angle)でright方向、sin(angle)でforward方向の成分を計算
    vector3 offset = first_right_ * (std::cos(angle) * dist) + 
                     first_forward_ * (std::sin(angle) * dist);
    
    // ターゲット座標を設定
    target = center + offset;
    target_point_ = target;
	*/

}

