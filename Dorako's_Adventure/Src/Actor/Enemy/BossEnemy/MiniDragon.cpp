#include "MiniDragon.h"
#include "../../../World/IWorld.h"
#include "../../../Collider/BoundingSphere.h"
#include "../EnemyState/EnemyStateChase.h"
#include "../EnemyState/EnemyStateIdle.h"
#include "../EnemyState/EnemyStateAttack.h"
#include "../EnemyState/EnemyStateDamage.h"
#include "../EnemyState/EnemyStateDead.h"
#include "../../../Delay/DelayManager.h"
#include "../../../Assets.h"

const float IdleTime = 60.0f;
// 距離の閾値
const float CloseDistance = 10.0f;   // 近い判定
const float FarDistance = 30.0f;     // 遠い判定
const float SafeDistance = 20.0f;    // 安全距離（空中時の逃げる目標）
//最大の移動スピード
const float MaxSpeed = 0.5f;

MiniDragon::MiniDragon(IWorld* world, GSvector3 position) :
	Enemy{Mesh_MiniDragon}
{
	world_ = world;
	name_ = "MiniDragon";
	tag_ = "BossEnemy";
	collider_ = BoundingSphere{ 1.0f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	foot_offset_ = 2.0f;
	health_ = 3;
	state_.add_state(EnemyState::Idle, new EnemyStateIdle(this));
	state_.add_state(EnemyState::Chase, new EnemyStateChase(this));
	state_.add_state(EnemyState::Attack, new EnemyStateAttack(this));
	state_.add_state(EnemyState::Damage, new EnemyStateDamage(this));
	state_.add_state(EnemyState::Dead, new EnemyStateDead(this));
	state_.change_state(EnemyState::Idle);
}
void MiniDragon::update(float delta_time) {
	//キャラクターの基礎アップデート
	state_.update(delta_time);
	collide_field();
	gravity_update(delta_time);
	collide_ground();
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());
	//アクターのプレイヤーを代入
	player_ = world_->find_actor("Player");

}
void MiniDragon::draw()const {
	mesh_->draw();
}
void MiniDragon::react(Actor& other) {
	
}
void MiniDragon::idle(float delta_time) {
	idle_timer_ += delta_time;
	if (idle_timer_ >= IdleTime) {
		change_state(EnemyState::Chase);
	}
}
void MiniDragon::chase(float delta_time) {

}
void MiniDragon::attack(float delta_time) {

}
void MiniDragon::damage(float delta_time) {

}
void MiniDragon::dead(float delta_time) {

}
void MiniDragon::perform_fire_attack() {

}
void MiniDragon::perform_charge_attack() {

}
void MiniDragon::perform_escape_action() {

}
void MiniDragon::build_attack_behavior_tree() {
	auto root = std::make_unique<SelectorNode>("攻撃行動ルート");
	auto initial_breath_sequence = std::make_unique<SequenceNode>("初回ブレス");
	initial_breath_sequence->add_child(std::make_unique<ConditionNode>([this]() {
		return !has_fired_fire_attack_;
		},
		"初回ブレス未実装？"));
}