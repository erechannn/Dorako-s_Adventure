#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include "../../Shape/Line.h"
#include "../../Assets.h"

Enemy::Enemy(GSuint mesh) :
	Actor{ mesh } {

}
void Enemy::collide_field() {
	// �ǂƂ̏Փ˔���i���̂Ƃ̔���j
	GSvector3 center; // �����߂���̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		// �␳��̍��W�ɕύX����
		transform_.position(center);
	}
}
void Enemy::collide_actor(Actor& other) {
	// �����W���������W�����߂�
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// ����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	// �Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius + other.collider().radius;
	// �Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	// �d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// �t�B�[���h�Ƃ̏Փ˔���
	collide_field();
}
//�d��
void Enemy::gravity_update(float delta_time) {
	GSvector3 planet_position{ 0.0f,-20.0f,0.0f };//���̒��S
	GSvector3 position = transform_.position();//�����̈ʒu
	GSvector3 gravity = position - planet_position;//�����x�N�g�������߂�
	gravity = gravity.normalize();//�P��x�N�g��
	gravity_velocity_ += gravity * gravity_ * delta_time;
	transform_.translate(gravity_velocity_, GStransform::Space::World);//�ړ�
}
void Enemy::collide_ground() {
	// �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
	GSvector3 line_start = transform_.position() + transform_.up() * enemy_height_;
	GSvector3 down_direction = -transform_.up();
	GSvector3 line_end = transform_.position() + (down_direction * foot_offset_);
	GSvector3 collision_point;    // �Փ˂����n�ʂƂ̌�_
	GSplane ground_plane;         // �Փ˂����n�ʂ̕���
	if (gsOctreeCollisionLine(gsGetOctree(Octree_TestStageCollider),
		&line_start, &line_end, &collision_point, &ground_plane)) {
		// �Փ˂����ʒu�ɍ��W��␳����
		transform_.position(collision_point);
		// �Ζʂɍ��킹�ăL�����N�^���X������
		GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
		GSvector3 up = transform_.position() - planet_position;
		GSvector3 left = GSvector3::cross(up, transform_.forward());
		GSvector3 forward = GSvector3::cross(left, up);
		transform_.rotation(GSquaternion::lookRotation(forward, up));
	}
}