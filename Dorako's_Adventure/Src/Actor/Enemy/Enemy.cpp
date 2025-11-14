#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include "../../Shape/Line.h"
#include "../../Assets.h"

Enemy::Enemy(GSuint mesh) :
	Character{ mesh } {

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
	GSvector3 planet_position = { 0.0f,-20.0f,0.0f };
	//惑星から自分までのベクトル
	GSvector3 to_planet = transform_.position() - planet_position;
	to_planet.normalize();
	//仮の上方向ベクトル
	GSvector3 up = to_planet;
	//惑星の外枠
	to_planet = to_planet * 20.0f;
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
