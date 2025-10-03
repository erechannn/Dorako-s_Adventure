#include "Player.h"
#include "Assets.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "../../Shape/Line.h"
#include "PlayerMotion.h"
#include "PlayerState/PlayerStateDamage.h"
#include "PlayerState/PlayerStateMove.h"
#include "PlayerState/PlayerStateJump.h"
#include <imgui/imgui.h>

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

	state_.add_state(PlayerState::StateDamage, new PlayerStateDamage(this));
	state_.add_state(PlayerState::StateMove, new PlayerStateMove(this));
	state_.add_state(PlayerState::StateJumpStart, new PlayerStateJump(this));
	state_.change_state(PlayerState::StateMove);

}
void Player::update(float delta_time) {
	state_.update(delta_time);
	//メッシュのモーションを更新
	mesh_->update(delta_time);
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());
	//フィールドとの当たり判定
	collide_field();
	gravity_update(delta_time);
	//プレイヤーを地面に対して垂直に立たせる
	collide_ground();

	//プレイヤーの状態管理
	if (state_.now_state_ == PlayerState::StateMove && gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_A)) {
		//Aボタンでジャンプ
		gravity_ = 0.016f;
		state_.change_state(PlayerState::StateJumpStart);
	}

	//動ける状態か
	if (state_.now_state_ == PlayerState::StateMove || state_.now_state_ == PlayerState::StateJumpStart) {
		is_move_ = true;
	}
	else is_move_ = false;


	//デバック表示
	ImGui::Begin("Player");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);

	ImGui::End();
}
void Player::draw()const {
	mesh_->draw();
}
void Player::react(Actor& other) {
}
void Player::move(float delta_time) {
	GSvector2 result;
	//左スティックの取得
	gsXBoxPadGetLeftAxis(0, &result);
	//スティックの移動量の数値化
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);

	if (!is_move_)return;
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward = transform_.inverseTransformVector(forward);//
	forward.y = 0.0f;
	forward = forward.normalize();
	GSvector3 right = world_->camera()->transform().right();
	right = transform_.inverseTransformDirection(right);
	right.y = 0.0f;
	right = right.normalize();
	// スティックの移動値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	//右スティックで移動
	if (result.y > 0.0f) velocity += forward;
	if (result.y < 0.0f) velocity -= forward;
	if (result.x < 0.0f) velocity -= right;
	if (result.x > 0.0f) velocity += right;
	velocity = velocity.normalized() * WalkSpeed  *result_normalize* delta_time;
	// 移動してなければアイドル状態
	GSint motion{ PlayerMotion::Idle };
	// 移動しているか？
	if (velocity.length() != 0.0f) {
		// 向きの補間
		GSvector3 velocity_world = transform_.transformDirection(velocity);
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.localRotation(),
				GSquaternion::lookRotation(velocity_world), 15.0f * delta_time);
		transform_.rotation(rotation);
		if (result_normalize <= 0.2f) {
			motion = PlayerMotion::Walk;
		}
		else  {
			motion = PlayerMotion::Run;
		}
	}

	velocity = transform_.transformDirection(velocity);



	// 移動量のxz成分だけ更新
	velocity_ = velocity;

	mesh_->change_motion(motion, true);

	//平行移動する（ワールド基準）
	transform_.translate(velocity, GStransform::Space::World);

	
}
void Player::jump(float delta_time) {
	if (state_.now_state_ != PlayerState::StateJumpStart) return;
	
	

}void Player::flying(float delta_time) {

}
void Player::landing(float delta_time) {

}
void Player::attack() {

}
bool Player::is_motion_end() {
	return mesh_->is_end_motion();
}