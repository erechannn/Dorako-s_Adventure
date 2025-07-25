#include"Player.h"
#include"Assets.h"
#include"World/IWorld.h"
#include"World/Field.h"
#include"Input/GamePadInput.h"
#include"PlayerMotion.h"
#include"PlayerState/PlayerStateDamage.h"

const float PlayerHeight{ 1.0f };
const float PlayerRadius{ 0.5f };
const float FootOffset{ 0.1f };
const float WalkSpeed{ 0.25f };

Player::Player(IWorld* world, GSvector3 position) :
	Actor{ Mesh_Player } {
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0,PlayerHeight,0} };
	transform_.position(position);

	state_.add_state(PlayerState::StateDamage, new PlayerStateDamage(this));
}
void Player::update(float delta_time) {
	state_.update(delta_time);
}
void Player::draw()const {

}
void Player::react(Actor& other) {

}
GSvector2 Player::move_input() {
	return enable_collider_ ? GamePadInput::get_stick(PadVec2::LeftStick, 0) : GSvector2::zero();
}
void Player::move(float delta_time) {
	GSvector2 result;
	//左スティックの取得
	gsXBoxPadGetLeftAxis(0, &result);
	//スティックの移動量の数値化
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);

	if (state_.now_state_ != PlayerState::StateMove)return;
	// カメラの前方向ベクトルを取得
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
	// スティックの移動値から移動ベクトルを計算
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	//右スティックで移動
	if (result.y > 0.0f) velocity += forward;
	if (result.y < 0.0f) velocity -= forward;
	if (result.x < 0.0f) velocity -= right;
	if (result.x > 0.0f) velocity += right;
	velocity = velocity.normalized() * WalkSpeed * result_normalize * delta_time;
	// 移動してなければアイドル状態
	GSint motion{ PlayerMotion::Idle };
	// 移動しているか？
	if (velocity.length() != 0.0f) {
		// 向きの補間
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 100.0f * delta_time);
		transform_.rotation(rotation);
		if (result_normalize <= 0.3f) {
			motion = PlayerMotion::Walk;
		}
		else if (result_normalize <= 0.8) {
			motion = PlayerMotion::Run;
		}
	}

	// 移動量のxz成分だけ更新
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;



	//平行移動する（ワールド基準）
	transform_.translate(velocity_, GStransform::Space::World);
}
