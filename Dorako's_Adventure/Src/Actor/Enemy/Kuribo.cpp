#include "Kuribo.h"
#include "../../World/IWorld.h"
#include "../../Assets.h"
#include "../../Collider/BoundingSphere.h"
#include "EnemyState/EnemyStateSearch.h"
#include "EnemyState/EnemyStateChase.h"
#include "EnemyState/EnemyStateIdle.h"
#include "../../Delay/DelayManager.h"
#include <imgui/imgui.h>
#include <algorithm>
#include <iostream>

const float M_PI{ 3.14159265358979323846 };

Kuribo::Kuribo(IWorld* world, GSvector3 position) :
	Enemy{ Mesh_Kuribo },
	enemy_eye_{world_,transform_.position(),transform_.forward(),60.0f,10.0f} {
	world_ = world;
	name_ = "Kuribo";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere(1.0f, GSvector3{ 0.0f,height_,0.0f });
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	foot_offset_ = 2.0f;
	first_transform_ = transform_;
	set_next_point();
	state_.add_state(EnemyState::Idle, new EnemyStateIdle(this));
	state_.add_state(EnemyState::Search, new EnemyStateSearch(this));
	state_.add_state(EnemyState::Chase, new EnemyStateChase(this));
	state_.change_state(EnemyState::Search);
}
void Kuribo::update(float delta_time) {
	state_.update(delta_time);
	collide_field();
	gravity_update(delta_time);
	collide_ground();
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());

	player_ = world_->find_actor("Player");

	enemy_eye_.setOrigin(transform_.position());
	enemy_eye_.setForward(transform_.forward());

	if (is_player_in_sight()&&is_chase_) {
		change_state(EnemyState::Chase);
	}

	bool next_point = false;

	float dis = GSvector3::distance(target_point_, transform_.position());

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
	ImGui::Checkbox("is_chase:", &is_chase_);
	ImGui::End();

	if (next_point) {
		set_next_point();
		next_point = false;

	}

}
void Kuribo::draw()const {
	mesh_->draw();
	enemy_eye_.draw();
	test_.draw();
	test2_.draw();
}
void Kuribo::react(Actor& other) {
	if (is_above_player(other)&&!undead_&&other.tag()=="PlayerTag") {
		die();
	}
	if (other.tag() == "PlayerTag") {
		first_position_ = transform_.position();
		change_state(EnemyState::Idle);
	}
}
void Kuribo::idle(float delta_time) {
	transform_.position(first_position_);
	Delay::after(1.5f, [this]() {
		if (this->is_player_in_sight()) {
			this->change_state(EnemyState::Chase);
		}
		else {
			this->change_state(EnemyState::Search);
		}
		});
}

void Kuribo::search(float delta_time) {

	//とりあえずまっすぐに移動
	if (is_move_) {
		//目的地に近づいたら次の目的地に設定
		float dis = GSvector3::distance(target_point_, transform_.position());
		if (dis <= 0.3f) {
			set_next_point();
		}
		to_target(delta_time, target_point_);
	}
}
void Kuribo::chase(float delta_time) {
	target_point_ = player_->transform().position();

	to_target(delta_time, target_point_);
	if (!is_player_in_sight()) {
		change_state(EnemyState::Search);
		set_next_point();
	}
}


bool Kuribo::is_above_player(Actor& other) {
	GSvector3 position = transform_.position();
	GSvector3 other_position = other.transform().position();
	GSvector3 up = transform_.up();
	GSvector3 to_target = other_position - position;
	float dot = to_target.dot(up);
	float threshold = 0.7f;
	return dot > threshold;
}

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
	target.x = center.x + std::cos(angle) * radius;
	target.z = center.z + std::sin(angle) * radius;
	target = first_transform_.transformPoint(target);

	GSvector3 planet_position = { 0.0f,-20.0f,0.0f };
	target = target - planet_position;
	target = target.normalize();
	target = target * 20.0f;
	target = target + planet_position;

	test_ = BoundingSphere{ 1.0f,target };
	target_point_ = target;
}
bool Kuribo::is_player_in_sight() {
	if (player_ == nullptr)return false;
	if (enemy_eye_.isTargetWithin(player_->transform().position())) {
		return true;
	}
	else return false;
}

