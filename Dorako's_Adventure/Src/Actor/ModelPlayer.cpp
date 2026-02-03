#include "ModelPlayer.h"
#include "Player/PlayerMotion.h"
#include "../Stage/StageManager.h"
#include "../World/IWorld.h"
#include "../Assets.h"

#include<imgui/imgui.h>

const float WalkSpeed{ 0.15f };

ModelPlayer::ModelPlayer(IWorld* world, GSvector3 position) :
	Character{ Mesh_Player } {
	world_ = world;
	name_ = "ModelPlayer";
	tag_ = "ModelTag";
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	mesh_->change_motion(PlayerMotion::Run,true);
	target_points_[0] = { 0.0f,-5.0f,10.0f };
	target_points_[1] = { 0.0f,-5.0f,-10.0f };
	target_point_ = { 0.0f,-4.7f,13.0f };
}
void ModelPlayer::update(float delta_time) {
	mesh_->update(delta_time);
	//フィールドとの当たり判定
	collide_field();
	gravity_update(delta_time);
	//プレイヤーを地面に対して垂直に立たせる
	collide_ground();
	//ワールド変換行列を設定
	mesh_->transform(transform_.localToWorldMatrix());

	//目的地に近づいたら次の目的地に設定
	float dis = GSvector3::distance(target_point_, transform_.position());
	if (dis <= 0.3f) {
		target_point_.z *= -1.0f;
	}
	//目的地に移動
	to_target(target_point_,delta_time);

}
void ModelPlayer::draw()const {
	mesh_->draw();
}
void ModelPlayer::react(Actor& other) {

}

void ModelPlayer::to_target(GSvector3 target, float delta_time) {
	//目的地までのベクトル
	GSvector3 to_target = target - transform_.position();
	GSvector3 target_normal = to_target.normalize();
	//移動量の計算
	velocity_ = target_normal * WalkSpeed * delta_time;
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