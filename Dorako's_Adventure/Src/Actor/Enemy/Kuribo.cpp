#include "Kuribo.h"
#include "../../World/IWorld.h"
#include "../../Assets.h"
#include "../../Collider/BoundingSphere.h"
#include "EnemyState/EnemyStateSearch.h"
#include "EnemyState/EnemyStateChase.h"
#include "EnemyState/EnemyStateIdle.h"
#include "EnemyState/EnemyStateDead.h"
#include "../../Delay/DelayManager.h"
#include <imgui/imgui.h>
#include <algorithm>
#include <iostream>


Kuribo::Kuribo(IWorld* world, GSvector3 position) :
	Enemy{ Mesh_Kuribo },
	enemy_eye_{world_,transform_.position(),transform_.forward(),60.0f,10.0f} {
	world_ = world;
	name_ = "Kuribo";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere(1.0f, GSvector3{ 0.0f,height_,0.0f });
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	//バグ防止のために足元の設定を高くする
	foot_offset_ = 2.0f;
	//ずれの修正用の変数
	base_position_ = position;
	//ランダムな目的地を設定するための中心
	first_transform_ = transform_;
	//ランダムに目的地を設定
	set_next_point();
	//ステートマシン
	state_.add_state(EnemyState::Idle, new EnemyStateIdle(this));
	state_.add_state(EnemyState::Search, new EnemyStateSearch(this));
	state_.add_state(EnemyState::Chase, new EnemyStateChase(this));
	state_.add_state(EnemyState::Dead, new EnemyStateDead(this));
	state_.change_state(EnemyState::Idle);
}
void Kuribo::update(float delta_time) {
	//アクターのプレイヤーを代入
	player_ = world_->find_actor("Player");
	//キャラクターの基礎アップデート
	state_.update(delta_time);
	collide_field();
	gravity_update(delta_time);
	collide_ground();
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());
	//視界の更新
	enemy_eye_.setOrigin(transform_.position());
	enemy_eye_.setForward(transform_.forward());


	//デバック用の処理
	//bool next_point = false;
	//float dis = GSvector3::distance(target_point_, transform_.position());
	//if (next_point) {
	//	set_next_point();
	//	next_point = false;

	//}
	//デバック表示
	//ImGui::Begin("Kuribo");
	//ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	//ImGui::Text("x:%f y:%f z:%f", transform_.up().x, transform_.up().y, transform_.up().z);
	//ImGui::Text("x:%f y:%f z:%f", transform_.forward().x, transform_.forward().y, transform_.forward().z);
	//ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);
	//ImGui::Text("x:%f y:%f z:%f", target_point_.x, target_point_.y, target_point_.z);
	//ImGui::Text("distance%f",dis);
	//ImGui::Checkbox("is_move:",&is_move_);
	//ImGui::Checkbox("is_ground", &is_ground_);
	//ImGui::Checkbox("next: ", &next_point);
	//ImGui::Checkbox("is_chase:", &is_chase_);
	//ImGui::End();
}
void Kuribo::draw()const {
	mesh_->draw();
}
void Kuribo::react(Actor& other) {
	//プレイヤーが上から当たったら死ぬ
	if (other.tag()=="PlayerAttackTag" || is_above_player(other)  && other.tag() == "PlayerTag") {
		is_dead_ = true;
		base_position_ = transform_.position();
		gsPlaySE(SE_DamageEnemy);
		change_state(EnemyState::Dead);
	}
	else if (other.tag() == "PlayerTag") {
		base_position_ = transform_.position();
		change_state(EnemyState::Idle);
	}
}
void Kuribo::idle(float delta_time) {
	if (player_ == nullptr)return;
	transform_.position(base_position_);
	//少し止まる(90フレーム)
	if (idle_timer_ >= 90.0f) {
		if (this->is_player_in_sight()) {			//プレイヤーが近くにいたら
			this->change_state(EnemyState::Chase);	//追いかける
		}
		else {										//それ以外なら
			this->change_state(EnemyState::Search); //徘徊する
		}

	}
	idle_timer_ += delta_time;
}

void Kuribo::search(float delta_time) {
	//目的地に近づいたら次の目的地に設定
	float dis = GSvector3::distance(target_point_, transform_.position());
	if (dis <= 0.3f) {
		set_next_point();
	}
	//目的地に移動
	to_target(delta_time, target_point_);
	//視界内にプレイヤーがいたら追いかける状態に遷移
	if (is_player_in_sight() && is_chase_) {
		change_state(EnemyState::Chase);
	}
}
void Kuribo::chase(float delta_time) {
	//ターゲットをプレイヤーに変更
	target_point_ = player_position();
	//プレイヤーの位置へ移動
	to_target(delta_time, target_point_);
	//プレイヤーが視界から消えたらサーチ状態に戻る
	if (!is_player_in_sight()) {
		velocity_ = GSvector3::zero();
		base_position_ = transform_.position();
		set_next_point();
		change_state(EnemyState::Idle);
	}
}
void Kuribo::dead(float delta_time) {

}
//敵の視界にプレイヤーが入ったか
bool Kuribo::is_player_in_sight() {
	if (player_ == nullptr)return false;
	if (enemy_eye_.isTargetWithin(player_position())) {
		return true;
	}
	else return false;
}
GSvector3 Kuribo::player_position() {
	if (player_ == nullptr)return { 0.0f,0.0f,0.0f };
	return player_->transform().position();
}
