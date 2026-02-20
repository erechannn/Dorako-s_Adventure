#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include "../../Shape/Line.h"
#include "../../Assets.h"
#include "../Item/FireItem.h"
#include "../Coin/Coin.h"

#include <iostream>

const float M_PI{ 3.14159265358979323846 };

Enemy::Enemy(GSuint mesh) :
	Character{ mesh } {

}
bool Enemy::is_above_player(Actor& player) {
	if (is_dead())return false;
	if (player.is_dead())return false;
	GSvector3 position = transform_.position();
	GSvector3 other_position = player.transform().position();
	GSvector3 up = transform_.up();
	GSvector3 to_target = other_position - position;
	float dot = to_target.dot(up);
	float threshold = 0.7f;
	return dot > threshold;

}

void Enemy::to_target(float delta_time,GSvector3 target) {
	//目的地までのベクトル
	GSvector3 to_target = target - transform_.position();
	GSvector3 target_normal = to_target.normalize();
	//移動量の計算
	velocity_ = target_normal * walk_speed_ * delta_time;
	//移動前の位置を記録
	GSvector3 position = transform_.position();
	//移動量を元に移動
	transform_.translate(velocity_, GStransform::Space::World);
	//惑星の位置
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	//惑星から自分までのベクトル
	GSvector3 to_planet = transform_.position() - planet_position;
	to_planet.normalize();
	//仮の上方向ベクトル
	GSvector3 up = to_planet;
	//惑星の外枠
	float planet_radius = StageManager::get_instance().get_current_stage_planet_radius();
	to_planet = to_planet * planet_radius;
	to_planet = to_planet + planet_position;
	GSvector3 to_position = to_planet - position;
	to_position.normalize();
	GSvector3 left = GSvector3::cross(up, to_position);
	GSvector3 forward = GSvector3::cross(left, up);
	//正しい場所に訂正
	transform_.position(to_planet);
	//向きを移動方向に回転
	transform_.rotation(GSquaternion::lookRotation(forward, up));
}
void Enemy::set_next_point() {
	GSvector3 center = first_transform_.inverseTransformPoint(first_transform_.position());
	GSvector3 forward = transform_.forward();
	GSvector3 right = transform_.right();
	GSvector3 target;
	float radius = 3.0f;

	// ランダムな角度（0～2π）
	float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;

	// ランダムな距離（0.0～radius）
	float dist = static_cast<float>(std::rand()) / RAND_MAX * radius;

	// ターゲット座標を設定
	target.x = center.x + std::cos(angle) * radius;
	target.z = center.z + std::sin(angle) * radius;
	target = first_transform_.transformPoint(target);

	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	target = target - planet_position;
	target = target.normalize();
	float planet_radius = StageManager::get_instance().get_current_stage_planet_radius();
	target = target * planet_radius;
	target = target + planet_position;

	target_point_ = target;

}
void Enemy::up_vector_update() {
	//惑星の位置
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	//惑星から自分までのベクトル
	GSvector3 to_planet = transform_.position() - planet_position;
	to_planet.normalize();
	//仮の上方向ベクトル
	GSvector3 up = to_planet;
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	GSvector3 forward = GSvector3::cross(left, up);

	//向きを移動方向に回転
	transform_.rotation(GSquaternion::lookRotation(forward, up));

}
void Enemy::spawn_item(int item) {
	if (item == Item::Fire) {
		world_->add_actor(new FireItem (world_, transform_.position()));
	}
	if (item == Item::CoinItem) {
		world_->add_actor(new Coin(world_, transform_.position()));
	}
}