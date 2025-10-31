#include "Player.h"
#include "Assets.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "../../Shape/Line.h"
#include "../BulletClass.h"
#include "PlayerMotion.h"
#include "PlayerState/PlayerStateDamage.h"
#include "PlayerState/PlayerStateMove.h"
#include "PlayerState/PlayerStateJump.h"
#include "PlayerState/PlayerStateFlying.h"
#include "PlayerState/PlayerStateJumpEnd.h"
#include "PlayerState/PlayerStateDead.h"
#include <imgui/imgui.h>
#include <iostream>

const float PlayerHeight{ 1.0f };
const float PlayerRadius{ 0.5f };
const float WalkSpeed{ 0.15f };

Player::Player(IWorld* world, GSvector3 position) :
	Character{ Mesh_Player } {
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0,PlayerHeight,0} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	max_health_ = 300000;
	health_ = max_health_;
	//状態の追加
	state_.add_state(PlayerState::StateDamage, new PlayerStateDamage(this));
	state_.add_state(PlayerState::StateMove, new PlayerStateMove(this));
	state_.add_state(PlayerState::StateJumpStart, new PlayerStateJump(this));
	state_.add_state(PlayerState::StateFlying, new PlayerStateFlying(this));
	state_.add_state(PlayerState::StateJumpEnd, new PlayerStateJumpEnd(this));
	state_.add_state(PlayerState::StateDead, new PlayerStateDead(this));
	state_.change_state(PlayerState::StateMove);

}
void Player::update(float delta_time) {
	state_.update(delta_time);
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//フィールドとの当たり判定
	collide_field();
	gravity_update(delta_time);
	//プレイヤーを地面に対して垂直に立たせる
	collide_ground();
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());

	//プレイヤーの状態管理
	if (state_.now_state_ == PlayerState::StateMove && gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_A)) {	//Aボタンでジャンプ

		state_.change_state(PlayerState::StateJumpStart);
	}

	//動ける状態か
	if (state_.now_state_ == PlayerState::StateMove || state_.now_state_ == PlayerState::StateFlying||state_.now_state_==PlayerState::StateJumpStart) {
		is_move_ = true;
	}
	else is_move_ = false;
	if(gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_B)&&is_move_){
		GSvector3 position = transform_.position() + transform_.forward();
		world_->add_actor(new BulletClass{ world_,position,transform_.forward(),Effeck_FireBoll,"PlayerAttackTag","PlayerFireAttack",tag_});
	}


	//デバック表示
	ImGui::Begin("Player");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("gravity:%f", gravity_);
	ImGui::Checkbox("地上にいるか", &is_ground_);

	ImGui::End();
}
void Player::draw()const {
	//メッシュの表示
	mesh_->draw();
	GSvector3 up = transform_.up();
	std::cout << "2 x: " << up.x << " y: " << up.y << " z: " << up.z << std::endl;

}
//当たり判定
void Player::react(Actor& other) {
	//特になし
	if (other.tag() == "EnemyTag") {
		if (is_above_enemy(other)) {
			state_.change_state(PlayerState::StateJumpStart);
		}
		else {
			//ノックバック
			GSvector3 now_pos{ transform().position() };
			GSvector3 knock_pos{ now_pos };
			knock_pos -= transform().forward() * 2.0f;
			// 補正後の座標に変更する
			transform_.position(knock_pos);
			health_ -= 1;
			state_.change_state(PlayerState::StateDamage);

		}
	}
}
void Player::move(float delta_time) {
	//スティックの2次元ベクトル
	GSvector2 result;
	//左スティックの取得
	gsXBoxPadGetLeftAxis(0, &result);
	//スティックの移動量の数値化
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);
	//動ける状態か
	if (!is_move_)return;
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward = transform_.inverseTransformVector(forward);//ワールドからローカルに
	forward.y = 0.0f;//Yは無効
	forward = forward.normalize();//正規化
	//カメラの右向きベクトルを取得
	GSvector3 right = world_->camera()->transform().right();
	right = transform_.inverseTransformDirection(right);//ワールドからローカル
	right.y = 0.0f;//Yは無効
	right = right.normalize();//正規化
	// スティックの移動値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	//右スティックで移動
	//前後ろに倒して前方向に加算し移動
	if (result.y > 0.0f) velocity += forward;
	if (result.y < 0.0f) velocity -= forward;
	//右左に倒して右方向に加算し移動
	if (result.x < 0.0f) velocity -= right;
	if (result.x > 0.0f) velocity += right;
	//移動量を倒した量によってスピードを調整
	velocity = velocity.normalized() * WalkSpeed  *result_normalize* delta_time;
	// 移動してなければアイドル状態
	GSint motion{ PlayerMotion::Idle };
	// 移動しているか？
	if (velocity.length() != 0.0f) {
		// 移動方向に向く
		GSvector3 velocity_world = transform_.transformDirection(velocity);//ローカルからワールド
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity_world,transform_.up()), 15.0f * delta_time);
		transform_.rotation(rotation);
		//プレイヤーの移動量に合わせてモーションの変化
		if (result_normalize <= 0.2f) {
			motion = PlayerMotion::Walk;
		}
		else  {
			motion = PlayerMotion::Run;
		}
	}
	//移動量をローカルからワールドに
	velocity = transform_.transformDirection(velocity);
	if (!is_ground_) {
		motion = PlayerMotion::Flying;
		GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
		GSvector3 up = transform_.position() - planet_position;
		GSvector3 left = GSvector3::cross(up, transform_.forward());
		GSvector3 forward = GSvector3::cross(left, up);
		transform_.rotation(GSquaternion::lookRotation(forward, up));

	}
	// 移動量のxz成分だけ更新
	velocity_ = velocity;
	if (state_.now_state_ != PlayerState::StateJumpStart) {
		//モーションの変更
		mesh_->change_motion(motion, true);


	}
	//平行移動する（ワールド基準）
	transform_.translate(velocity, GStransform::Space::World);

	
}
void Player::flying(float delta_time) {
	if (gsXBoxPadButtonState(0, GS_XBOX_PAD_A)) {
		is_zero_gravity_ = true;
	}
	else is_zero_gravity_ = false;
	move(delta_time);
}
void Player::landing(float delta_time) {

}
void Player::attack() {

}
bool Player::is_motion_end() {
	return mesh_->is_end_motion();
}
bool Player::is_above_enemy(Actor&other) {
	GSvector3 position = transform_.position();
	GSvector3 other_position = other.transform().position();
	GSvector3 other_up = other.transform().up();
	GSvector3 to_other = position - other_position;
	float dot = to_other.dot(other_up);
	return dot > 0;
}