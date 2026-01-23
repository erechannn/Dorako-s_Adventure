#include "BulletClass.h"
#include "../World/IWorld.h"
#include "../Assets.h"
#include "../World/Field.h"
#include "../Shape/Line.h"
#include "../Stage/StageManager.h"
#include <GSeffect.h>

BulletClass::BulletClass(IWorld* world, const GSvector3& position, const GSvector3& velocity, GSint effect_handle,
	const std::string& tag,const std::string& name,const std::string& owner_tag) :
	Character{ 0 } //メッシュ無し
{
	world_ = world;
	tag_ = tag;
	name_ = name;
	transform_.forward(velocity);
	collider_ = BoundingSphere{ 0.5,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	owner_tag_ = owner_tag;
	lifespan_timer_ = 300.0f;
	//炎の弾のエフェクト
	effect_handle_ = gsPlayEffect(effect_handle, &position);
}

void BulletClass::update(float delta_time) {
	//持続時間がなくなったら消す
	if (lifespan_timer_ <= 0.0f) {
		gsStopEffect(effect_handle_);
		die();
		return;
	}
	//持続時間を減らす
	lifespan_timer_ -= delta_time;

	//地面にそらして回転させる
	GSvector3 planet_position = StageManager::get_instance().get_current_stage_planet_position();
	GSvector3 up = transform_.position() - planet_position;
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	GSvector3 forward = GSvector3::cross(left, up);
	transform_.rotation(GSquaternion::lookRotation(forward, up));
	//江ファクトの更新
	GSmatrix4 world = transform_.localToWorldMatrix();
	world.translate(transform_.up());
	gsSetEffectMatrix(effect_handle_, &world); // ワールド変換行列を設定

	// フィールドとの衝突判定
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// 交点の座標に補正
		transform_.position(intersect);
		// フィールドに衝突したら死亡
		gsStopEffect(effect_handle_);
		die();
		return;
	}

	// 移動する（ワールド座標系基準）
	transform_.translate(transform_.forward()*speed_*delta_time, GStransform::Space::World);
}
void BulletClass::draw()const {
}
void BulletClass::react(Actor& other) {
	//出した本人自身と同じタグのオブジェクト以外に当たったら死亡
	if (other.tag() == tag() || other.tag() == owner_tag_) return;
	gsStopEffect(effect_handle_);
	die();
}
