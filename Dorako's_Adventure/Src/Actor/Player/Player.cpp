#include"Player.h"
#include"Assets.h"
#include"World/IWorld.h"
#include"World/Field.h"
#include"../../Shape/Line.h"
#include"PlayerMotion.h"
#include"PlayerState/PlayerStateDamage.h"
#include"PlayerState/PlayerStateMove.h"
#include"PlayerState/PlayerStateJump.h"
#include<imgui/imgui.h>
#include<iostream>

const float PlayerHeight{ 1.0f };
const float PlayerRadius{ 0.5f };
const float FootOffset{ 0.001f };
const float WalkSpeed{ 0.15f };

Player::Player(IWorld* world, GSvector3 position) :
	Actor{ Mesh_Player } {
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
	//重力
	GSvector3 planet_position{0.0f,-20.0f,0.0f};
	GSvector3 position = transform_.position();
	GSvector3 gravity = position - planet_position;
	GSvector3 gravity_normalize = gravity.normalize();
	gravity_vro += gravity_normalize*gravity_ * delta_time;
	transform_.translate(gravity_vro, GStransform::Space::World);
	//プレイヤーを地面に対して垂直に立たせる
	player_rotate(delta_time);
	//プレイヤーの状態管理
	if (state_.now_state_ == PlayerState::StateMove && gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_A)) {
		state_.change_state(PlayerState::StateJumpStart);
	}
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
	//GSvector3 forward = world_->camera()->transform().forward();
	//forward.y = 0.0f;
	//GSvector3 right = world_->camera()->transform().right();
	//right.y = 0.0f;
	//プレイヤー基準
	GSvector3 forward = transform_.forward();
	GSvector3 right = transform_.right();
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
				GSquaternion::lookRotation(velocity), 10.0f * delta_time);
		transform_.rotation(rotation);
		if (result_normalize <= 0.3f) {
			motion = PlayerMotion::Walk;
		}
		else if (result_normalize <= 0.8) {
			motion = PlayerMotion::Run;
		}
	}

	player_rotate(delta_time);

	velocity = transform_.transformDirection(velocity);

	// 移動量のxz成分だけ更新
	velocity_ = velocity;

	mesh_->change_motion(motion, true);

	//平行移動する（ワールド基準）
	transform_.translate(velocity_, GStransform::Space::World);

	
}
void Player::jump(float delta_time) {
	if (state_.now_state_ != PlayerState::StateJumpStart) return;
	

}void Player::flying(float delta_time) {

}
void Player::landing(float delta_time) {

}
void Player::attack() {

}
void Player::collide_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 押し戻し後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		// y座標は変更しない
		center.y = transform_.position().y;
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	GSvector3 down_direction = -transform_.up();
	float line_length = PlayerHeight * 0.5f + FootOffset;
	//Line line;
	//line.start = position + collider_.center;
	////line.end = position + (down_direction * line_length);
	//line.end = GSvector3{ 0.0f,-20.0f,0.0f };
	//GSvector3 intersect;  // 地面との交点
	//if (world_->field()->collide(line, &intersect)) {
	//	// 交点の位置からy座標のみ補正する
	//	position = intersect;
	//	// 座標を変更する
	//	transform_.position(position);
	//	// 重力を初期化する
	//	velocity_.y = 0.0f;
	//}

}
void Player::collide_actor(Actor& other) {
	// ｙ座標を除く座標を求める
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// 相手との距離
	float distance = GSvector3::distance(position, target);
	// 衝突判定球の半径同士を加えた長さを求める
	float length = collider_.radius + other.collider().radius;
	// 衝突判定球の重なっている長さを求める
	float overlap = length - distance;
	// 重なっている部分の半分の距離だけ離れる移動量を求める
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// フィールドとの衝突判定
	collide_field();

}
void Player::player_rotate(float delta_time) {
	// 地面との衝突判定（線分との交差判定）
	GSvector3 line_start = transform_.position() + collider_.center;
	GSvector3 down_direction = -transform_.up();
	float line_length = PlayerHeight * 0.5f + FootOffset;
	GSvector3 line_end = transform_.position() + (down_direction * line_length);
	GSvector3 collision_point;    // 衝突した地面との交点
	GSplane ground_plane;         // 衝突した地面の平面
	if (gsOctreeCollisionLine(gsGetOctree(Octree_TestStageCollider),
		&line_start, &line_end, &collision_point, &ground_plane)) {
		// 衝突した位置に座標を補正する
		transform_.position(collision_point);
		//std::cout << "x:" << ground_plane.normal.x << "y:" << ground_plane.normal.y << "z:" << ground_plane.normal.z << std::endl;
		std::cout << "x:" << collision_point.x << "y:" << collision_point.y << "z:" << collision_point.z << std::endl;
		//斜面方向の移動量を求める
		GSvector3 slope_velocity = GSvector3{ 0.0f, velocity_.y, 0.0 } - ground_plane.normal * ground_plane.normal.y * velocity_.y;
		
		// 斜面に合わせてキャラクタを傾かせる
		//GSvector3 up = GSvector3::rotateTowards(transform_.up(), ground_plane.normal, 0.1f * delta_time, 0.0);
		GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
		GSvector3 up = transform_.position() - planet_position;
		GSvector3 left = GSvector3::cross(up, transform_.forward());
		GSvector3 forward = GSvector3::cross(left, up);
		transform_.rotation(GSquaternion::lookRotation(forward, up));
	}

}