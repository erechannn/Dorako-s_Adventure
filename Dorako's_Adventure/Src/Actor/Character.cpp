#include "Character.h"
#include "../World/IWorld.h"
#include "../World/Field.h"
#include "../Shape/Line.h"
#include "../Stage/StageManager.h"
#include "../Assets.h"
#include <iostream>

Character::Character(GSuint mesh) :
	Actor{ mesh } {

}
void Character::change_state(int next_state) {
	state_.change_state(next_state);
}
void Character::collide_field() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		GSvector3 position = transform_.inverseTransformPoint(transform_.position());
		//center = transform_.inverseTransformPoint(center);
		// y座標は変更しない
		center.y = transform_.position().y;
		//center = transform_.transformPoint(center);
		// 補正後の座標に変更する
		transform_.position(center);
	}
	// 地面との衝突判定（線分との交差判定）
	GSvector3 position = transform_.position();
	Line line;
	line.start = position + collider_.center;
	line.end = position + GSvector3{ 0.0f, -foot_offset_, 0.0f };
	GSvector3 intersect;            // 地面との交点
	// 衝突したフィールド用アクター
	Actor* field_actor{ nullptr };
	// 親をリセットしておく
	transform_.parent(nullptr);
	if (world_->field()->collide(line, &intersect, nullptr, &field_actor)) {
		// 座標を変更する
		transform_.position(position);
		// フィールド用のアクタークラスと衝突したか？
		if (field_actor != nullptr) {
			// 衝突したフィールド用のアクターを親のトランスフォームクラスとして設定
			transform_.parent(&field_actor->transform());
		}
	}
}
void Character::collide_actor(Actor& other) {
	// ｙ座標を除く座標を求める
	GSvector3 position = transform_.position();
	GSvector3 target = other.transform().position();
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
void Character::gravity_update(float delta_time) {
	//地上についていたら無視
	if (is_ground_)return;
	if (is_zero_gravity_)return;
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	GSvector3 position = transform_.position();//自分の位置
	GSvector3 gravity = position - planet_position;//方向ベクトルを求める
	gravity = gravity.normalize();//単一ベクトル
	gravity_velocity_ += gravity * gravity_ * delta_time;
	transform_.translate(gravity_velocity_, GStransform::Space::World);//移動
}
void Character::collide_ground() {
	// 壁との衝突判定（球体との判定）
	GSvector3 center; // 衝突後の球体の中心座標
	if (world_->field()->collide(collider(), &center)) {
		GSvector3 position = transform_.inverseTransformPoint(transform_.position());
		//center = transform_.inverseTransformPoint(center);
		// y座標は変更しない
		center.y = transform_.position().y;
		//center = transform_.transformPoint(center);
		// 補正後の座標に変更する
		transform_.position(center);
	}
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	GSvector3 up = (transform_.position() - planet_position);
	up.normalize();
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	GSvector3 forward = GSvector3::cross(left, up);
	transform_.rotation(GSquaternion::lookRotation(forward, up));


	// 地面との衝突判定（線分との交差判定）
	GSvector3 line_start = transform_.position() + transform_.up() * height_;
	GSvector3 down_direction = -transform_.up();
	GSvector3 line_end = transform_.position() + (down_direction * foot_offset_);
	GSvector3 collision_point;    // 衝突した地面との交点
	GSplane ground_plane;         // 衝突した地面の平面
	UINT octree_collider = StageManager::get_instance().get_current_stage_collider();
	if (gsOctreeCollisionLine(gsGetOctree(octree_collider),
		&line_start, &line_end, &collision_point, &ground_plane)) {
		// 衝突した位置に座標を補正する
		transform_.position(collision_point);
		GSvector3 hit_position = collision_point;
		collider_point_ = collision_point;
		if (GSvector3::distance(hit_position,transform_.position()) >= 2.0f) {
			std::cout << "異常発生" << std::endl;
			std::cout << "x: " << hit_position.x << " y: " << hit_position.y << " z: " << hit_position.z << std::endl;

		}
		hit_position_ = hit_position;
		//重力を無効
		is_ground_ = true;
		gravity_velocity_ = GSvector3::zero();
		// 斜面に合わせてキャラクタを傾かせる

		GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
		GSvector3 up = (transform_.position() - planet_position);
		up.normalize();
		GSvector3 left = GSvector3::cross(up, transform_.forward());
		GSvector3 forward = GSvector3::cross(left, up);
		transform_.rotation(GSquaternion::lookRotation(forward, up));
	}
	else  is_ground_ = false;

}

bool Character::is_ground() {
	return is_ground_;
}
int Character::now_health() {
	return health_;
}
void Character::take_damage() {
	health_ -= 1;
}