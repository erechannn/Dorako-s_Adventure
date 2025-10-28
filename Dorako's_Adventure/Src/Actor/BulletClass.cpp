#include "BulletClass.h"
#include "../World/IWorld.h"
#include "../Assets.h"
#include "../World/Field.h"
#include "../Shape/Line.h"
#include <GSeffect.h>

BulletClass::BulletClass(IWorld* world, const GSvector3& position, const GSvector3& velocity, GSint effect_handle,
	const std::string& tag,const std::string& name,const std::string& owner_tag) :
	Character{ 0 } {
	world_ = world;
	tag_ = tag;
	name_ = name;
	velocity_ = velocity;
	collider_ = BoundingSphere{ 0.5,{0.0f,1.0f,0.0f} };
	transform_.position(position);
	owner_tag_ = owner_tag;
	lifespan_timer_ = 300.0f;
	effect_handle_ = gsPlayEffect(effect_handle, &position);
}

void BulletClass::update(float delta_time) {
	if (lifespan_timer_ <= 0.0f) {
		die();
		return;
	}

	GSmatrix4 world = transform_.localToWorldMatrix();
	world.translate(0.0f, 1.0f, 0.0f);
	gsSetEffectMatrix(effect_handle_, &world); // ���[���h�ϊ��s���ݒ�
	lifespan_timer_ -= delta_time;
	// �t�B�[���h�Ƃ̏Փ˔���
	Line line;
	line.start = transform_.position();
	line.end = transform_.position() + velocity_ * delta_time;
	GSvector3 intersect;
	if (world_->field()->collide(line, &intersect)) {
		// ��_�̍��W�ɕ␳
		transform_.position(intersect);
		// �t�B�[���h�ɏՓ˂����玀�S
		die();
		return;
	}
	//�n�ʂɂ��炵�ĉ�]������
	GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
	GSvector3 up = transform_.position() - planet_position;
	GSvector3 left = GSvector3::cross(up, transform_.forward());
	GSvector3 forward = GSvector3::cross(left, up);
	transform_.rotation(GSquaternion::lookRotation(forward, up));

	// �ړ�����i���[���h���W�n��j
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
}
void BulletClass::draw()const {
	collider_.draw();
}
void BulletClass::react(Actor& other) {
	if (other.tag() == tag() || other.tag() == owner_tag_) return;
	die();
}
