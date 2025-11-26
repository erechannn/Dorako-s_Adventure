#include "MiniDragon.h"
#include "../../../World/IWorld.h"
#include "../../../Collider/BoundingSphere.h"
#include "../EnemyState/EnemyStateChase.h"
#include "../EnemyState/EnemyStateIdle.h"
#include "../EnemyState/EnemyStateAttack.h"
#include "../EnemyState/EnemyStateDamage.h"
#include "../EnemyState/EnemyStateDead.h"
#include "../../../Delay/DelayManager.h"
#include "../../Player/PlayerState/PlayerState.h"
#include "../../../Assets.h"

const float IdleTime = 60.0f;
// 距離の閾値
const float CloseDistance = 3.0f;   // 近い判定
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
	//プレイヤーとの距離
	player_distance_ = GSvector3::distance(transform_.position(), player_->transform().position());
	Player* player = (Player*)player_;
	now_player_state_=player->get_player_now_state();

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
void MiniDragon::perform_melee_attack() {

}
void MiniDragon::build_attack_behavior_tree() {
	auto root = std::make_unique<SelectorNode>("攻撃行動ルート");
	auto initial_breath_sequence = std::make_unique<SequenceNode>("初回ブレス");
	initial_breath_sequence->add_child(std::make_unique<ConditionNode>([this]() {
		return !has_fired_fire_attack_;
		},
		"初回ブレス未実装？"));
	auto action_loop_selector = std::make_unique<SelectorNode>("行動ループ（距離判定）");
	auto close_range_sequence = std::make_unique<SequenceNode>("近距離行動");
	close_range_sequence->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ < CloseDistance;
		},
		("プレイヤーが近い？(距離<3.0f")
	));
	auto player_state_selector = std::make_unique<SelectorNode>("プレイヤーの状態による分岐");
	auto airborne_sequence = std::make_unique<SequenceNode>("空中時の対応");
	airborne_sequence->add_child(std::make_unique<ConditionNode>([this]() {
		return now_player_state_ == PlayerState::StateFlying;
		}, "プレイヤーは空中状態？"
	));
	auto airborne_action_selector = std::make_unique<SelectorNode>("空中時の行動選択");
	auto safe_distance_breath_seq = std::make_unique<SequenceNode>("安全距離到達時のブレス");
	safe_distance_breath_seq->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ >= SafeDistance;
		},
		"安全地帯に到着？"
	));
	safe_distance_breath_seq->add_child(std::make_unique<ActionNode>(
		[this]() {
			perform_fire_attack();
			return Status::Success;
		},
		"ブレス攻撃（安全地帯到着後）"
	));
	auto keep_distance_action = std::make_unique<ActionNode>(
		[this]() {
			perform_escape_action();
			return Status::Success;
		
		},
		"距離を置く"
	);
	airborne_action_selector->add_child(std::move(safe_distance_breath_seq));
	airborne_action_selector->add_child(std::move(keep_distance_action));

	airborne_sequence->add_child(std::move(airborne_action_selector));
	auto grounded_action = std::make_unique<ActionNode>(
		[this]() {
			perform_melee_attack();
			return Status::Success;
		},
		"近接攻撃実行"
	);
	player_state_selector->add_child(std::move(airborne_sequence));
	player_state_selector->add_child(std::move(grounded_action));

	close_range_sequence->add_child(std::move(player_state_selector));


	auto mid_range_sequence = std::make_unique<SequenceNode>("中距離攻撃");

	mid_range_sequence->add_child(std::make_unique<ActionNode>(
		[this]() {
			perform_fire_attack();
			return Status::Success;
		},
		"ブレス攻撃"
	));
	
	auto far_range_sequence = std::make_unique<SequenceNode>("遠距離行動");
	far_range_sequence->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ >= FarDistance;
		},
		"プレイヤーが遠い？"
	));

	far_range_sequence->add_child(std::make_unique<ActionNode>(
		[this]() {
			perform_charge_attack();
			return Status::Success;
		},
		"高速追跡"
	));

	action_loop_selector->add_child(std::move(close_range_sequence));
	action_loop_selector->add_child(std::move(mid_range_sequence));
	action_loop_selector->add_child(std::move(far_range_sequence));

	root->add_child(std::move(initial_breath_sequence));
	root->add_child(std::move(action_loop_selector));

	attack_behavior_tree_ = std::move(root);

}