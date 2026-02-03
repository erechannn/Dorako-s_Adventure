#include "MiniDragon.h"
#include "../../../World/IWorld.h"
#include "../../../Collider/BoundingSphere.h"
#include "../EnemyState/EnemyStateChase.h"
#include "../EnemyState/EnemyStateIdle.h"
#include "../EnemyState/EnemyStateAttack.h"
#include "../EnemyState/EnemyStateDamage.h"
#include "../EnemyState/EnemyStateDead.h"
#include "BossAttackState.h"
#include "AttackStateFirstFireAttack.h"
#include "../EnemiesMotion.h"
#include "../../../Delay/DelayManager.h"
#include "../../Player/PlayerState/PlayerState.h"
#include "../../Coin/Coin.h"
#include "../../../Assets.h"

#include <iostream>

const float IdleTime = 60.0f;
// 距離の閾値
const float CloseDistance = 3.0f;   // 近い判定
const float FarDistance = 15.0f;     // 遠い判定
const float SafeDistance = 20.0f;    // 安全距離（空中時の逃げる目標）
//最大の移動スピード
const float MaxSpeed = 0.5f;

MiniDragon::MiniDragon(IWorld* world, GSvector3 position) :
	Enemy{Mesh_MiniDragon}
{
	world_ = world;
	name_ = "MiniDragon";
	tag_ = "BossEnemyTag";
	collider_ = BoundingSphere{ 1.0f,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	foot_offset_ = 2.0f;
	walk_speed_ = 0.15f;
	health_ = 3;
	base_position_ = position;
	state_.add_state(EnemyState::Idle, new EnemyStateIdle(this));
	state_.add_state(EnemyState::Chase, new EnemyStateChase(this));
	state_.add_state(EnemyState::Attack, new EnemyStateAttack(this));
	state_.add_state(EnemyState::Damage, new EnemyStateDamage(this));
	state_.add_state(EnemyState::Dead, new EnemyStateDead(this));
	state_.add_state(EnemyState::FirstFireAttack, new AttackStateFirstFireAttack(this));
	state_.change_state(EnemyState::Idle);
	build_attack_behavior_tree();
}
void MiniDragon::update(float delta_time) {
	delta_time_ = delta_time;
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
	if (other.tag() == "PlayerAttackTag" || is_above_player(other) && other.tag() == "PlayerTag") {
		base_position_ = transform_.position();
		health_ -= 1;
		if (health_ <= 0) {
			change_state(EnemyState::Dead);
		}
		else change_state(EnemyState::Damage);
	}
	else if (other.tag() == "PlayerTag") {
		base_position_ = transform_.position();
		change_state(EnemyState::Idle);
	}

}
void MiniDragon::idle(float delta_time) {
	idle_timer_ += delta_time;
	if (idle_timer_ >= IdleTime) {
		idle_timer_ = 0.0f;
		change_state(EnemyState::Chase);
	}
}
void MiniDragon::chase(float delta_time) {
	target_point_ = player_->transform().position();
	to_target(delta_time, target_point_);
	if (player_distance_ <= CloseDistance) {
		base_position_ = transform_.position();
		change_state(EnemyState::Attack);
		saved_position_ = transform_.position();
	}
}
void MiniDragon::attack(float delta_time) {
	transform_.position(base_position_);
	attack_behavior_tree_->tick();
}
void MiniDragon::damage(float delta_time) {

}
void MiniDragon::dead(float delta_time) {
	GSvector3 coin_position = transform_.position();
	world_->add_actor(new Coin(world_, coin_position));
}
void MiniDragon::perform_fire_attack() {
	transform_.position(saved_position_);

	GSvector3 to_player = player_->transform().position() - transform_.position();
	float dot = GSvector3::dot(to_player, transform_.forward());
	if (dot > 1.0) {
		look_to_player(player_->transform().position());
	}
}
void MiniDragon::perform_charge_attack() {
	std::cout << "突進攻撃" << std::endl;
}
void MiniDragon::perform_escape_action() {
	std::cout << "逃げる" << std::endl;

}
void MiniDragon::perform_melee_attack() {
	std::cout << "近接攻撃" << std::endl;
}
void MiniDragon::build_attack_behavior_tree() {

	// ルートノード（ORロジック）
	auto root = std::make_unique<SelectorNode>();
	// ============================================
	// 【初回ブレス攻撃】
	// 戦闘開始時に一度だけ実行される初撃
	// ============================================
	auto initial_breath_sequence = std::make_unique<SequenceNode>();

	// 条件：まだブレス攻撃をしていない
	initial_breath_sequence->add_child(std::make_unique<ConditionNode>([this]() {
		return !has_fired_fire_attack_;
	}));

	//アクション：初回ブレスまでの待機
	initial_breath_sequence->add_child(std::make_unique<ActionNode>([this]() {
		change_motion(EnemiesMotion::Idle, true);
		idle_timer_ += delta_time_;
		if (idle_timer_ >= 60.0f) {
			motion_flag_ = true;
			idle_timer_ = 0.0f;
			return Status::Success;
		}
		return Status::Running;
		}));

	// アクション：ブレス攻撃モーション
	initial_breath_sequence->add_child(std::make_unique<ActionNode>(
		[this]() {
			change_motion(EnemiesMotion::Attack, false);
			if (mesh_->is_end_motion()) {
				motion_flag_ = true;
				has_fired_fire_attack_ = true;
				mesh_->change_motion(EnemiesMotion::Idle, true);
				return Status::Success;
			}
			return Status::Running;
		}));

	// ============================================
	// 【メイン行動ループ】
	// プレイヤーとの距離に応じて行動を選択
	// ============================================
	auto action_loop_selector = std::make_unique<SelectorNode>();

	// --------------------------------------------
   // 【近距離行動】距離 < CloseDistance
   // --------------------------------------------
	auto close_range_sequence = std::make_unique<SequenceNode>();

	// 条件：プレイヤーが近距離にいる
	close_range_sequence->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ < CloseDistance;
		}
	));

	// プレイヤーの状態で行動を分岐
	auto player_state_selector = std::make_unique<SelectorNode>();

	// ＜プレイヤーが空中の場合＞
	auto airborne_sequence = std::make_unique<SequenceNode>();

	// 条件：プレイヤーが飛行中
	airborne_sequence->add_child(std::make_unique<ConditionNode>(
		[this]() {
		return now_player_state_ == PlayerState::StateFlying;
		}
	));
	// 空中プレイヤーへの対応
	auto airborne_action_selector = std::make_unique<SelectorNode>();

	// パターンA：安全距離からブレス攻撃
	auto safe_distance_breath_seq = std::make_unique<SequenceNode>();
	safe_distance_breath_seq->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ >= SafeDistance;
		}
	));
	safe_distance_breath_seq->add_child(std::make_unique<ActionNode>(
		[this]() {
			change_motion(EnemiesMotion::Attack, false);
			if (mesh_->is_end_motion()) {
				motion_flag_ = true;
				has_fired_fire_attack_ = true;
				return Status::Success;
			}
			return Status::Running;
		}
	));
	auto keep_distance_action = std::make_unique<ActionNode>(
		[this]() {
			perform_escape_action();
			return Status::Success;
		
		}
	);
	airborne_action_selector->add_child(std::move(safe_distance_breath_seq));
	airborne_action_selector->add_child(std::move(keep_distance_action));

	airborne_sequence->add_child(std::move(airborne_action_selector));
	auto grounded_action = std::make_unique<ActionNode>(
		[this]() {
			perform_melee_attack();
			return Status::Success;
		}
	);
	player_state_selector->add_child(std::move(airborne_sequence));
	player_state_selector->add_child(std::move(grounded_action));

	close_range_sequence->add_child(std::move(player_state_selector));


	auto mid_range_sequence = std::make_unique<SequenceNode>();

	mid_range_sequence->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ >= CloseDistance &&
				player_distance_ < FarDistance;
		}
	));

	mid_range_sequence->add_child(std::make_unique<ActionNode>(
		[this]() {
			perform_fire_attack();
			return Status::Success;
		}
	));
	
	auto far_range_sequence = std::make_unique<SequenceNode>();
	far_range_sequence->add_child(std::make_unique<ConditionNode>(
		[this]() {
			return player_distance_ >= FarDistance;
		}
	));

	far_range_sequence->add_child(std::make_unique<ActionNode>(
		[this]() {
			perform_charge_attack();
			return Status::Success;
		}
	));

	// ============================================
	// ツリーの組み立て
	// ============================================
	action_loop_selector->add_child(std::move(close_range_sequence));
	action_loop_selector->add_child(std::move(mid_range_sequence));
	action_loop_selector->add_child(std::move(far_range_sequence));

	root->add_child(std::move(initial_breath_sequence));
	root->add_child(std::move(action_loop_selector));

	attack_behavior_tree_ = std::move(root);

}

void MiniDragon::look_to_player(GSvector3 target) {
	//惑星の位置
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	//惑星から自分までのベクトル
	GSvector3 to_planet = transform_.position() - planet_position;
	to_planet.normalize();
	//仮の上方向ベクトル
	GSvector3 up = to_planet;
	transform_.lookAt(target, up);
}
void MiniDragon::change_motion(GSuint motion, bool loop) {
	if (motion_flag_) {
		mesh_->change_motion(motion, loop);
		motion_flag_ = false;
	}
}