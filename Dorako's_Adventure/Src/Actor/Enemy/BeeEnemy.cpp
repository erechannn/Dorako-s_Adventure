#include "BeeEnemy.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include "EnemyState/EnemyStateIdle.h"
#include "EnemyState/EnemyStateSearch.h"
#include "../../Assets.h"

BeeEnemy::BeeEnemy(IWorld* world, GSvector3 position) :
Enemy{Mesh_Bee}
{
	world_ = world;
	name_ = "BeeEnemy";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere{ 1.0f,{0.0f,height_,0.0f} };
	transform_.position(position);
	transform_.position(transform_.up() * 5.0f);
	mesh_->transform(transform_.localToWorldMatrix());
	foot_offset_ = 0.0f;
	state_.add_state(EnemyState::Idle, new EnemyStateIdle(this));
	state_.add_state(EnemyState::Search, new EnemyStateSearch(this));
	state_.change_state(EnemyState::Idle);
}
void BeeEnemy::update(float delta_time) {
	//キャラクターの状態の更新
	state_.update(delta_time);
	collide_field();
	//メッシュの更新
	mesh_->update(delta_time);
	mesh_->transform(transform_.localToWorldMatrix());
	//飛んでいる時の重力を適応する
	flying_ground();
}
void BeeEnemy::draw()const {
	mesh_->draw();
}
void BeeEnemy::react(Actor& other) {
	//プレイヤーが上から当たったら死ぬ
	if (other.tag() == "PlayerAttackTag" || is_above_player(other) && other.tag() == "PlayerTag") {
		base_position_ = transform_.position();
		gsPlaySE(SE_DamageEnemy);
		change_state(EnemyState::Dead);
	}
	else if (other.tag() == "PlayerTag") {
		base_position_ = transform_.position();
		change_state(EnemyState::Idle);
	}
}
void BeeEnemy::idle(float delta_time) {
	//もし例外敵に呼ばれてしまった場合のため処理
	state_.change_state(EnemyState::Search);
}
void BeeEnemy::search(float delta_time) {
	transform_.translate(transform_.forward() * walk_speed_ * delta_time, GStransform::Space::World);
}
