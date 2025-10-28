#include "FieldActor.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include "../../Shape/Line.h"
#include "../../Shape/Ray.h"
#include "../../Collider/BoundingSphere.h"
#include "../../Assets.h"

// �`��
void FieldActor::draw() const {
    // ���b�V���̕`��
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(mesh_);
    glPopMatrix();
}

// ���C�Ƃ̏Փ˔���
bool FieldActor::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane) const {
    // ���C�̃p�����[�^������ɕϊ�
    Line line;
    line.start = ray.position;
    line.end = ray.position + (ray.direction.normalized() * max_distance);
    // �����Ƃ̏Փ˔�����s��
    return collide(line, intersect, plane);
}

// �����Ƃ̏Փ˔���
bool FieldActor::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
    // ���[�J�����W�n�̐��������߂�
    Line local_line;
    local_line.start = transform_.inverseTransformPoint(line.start);
    local_line.end = transform_.inverseTransformPoint(line.end);
    // ���b�V���Ɛ����������������H
    if (gsCollisionMeshAndLine(gsGetMesh(mesh_collider_), &local_line.start, &local_line.end, intersect, plane) == GS_TRUE) {
        if (intersect != nullptr) {
            // ��_�����[���h���W�n�ɕϊ�
            *intersect = transform_.transformPoint(*intersect);
        }
        if (plane != nullptr) {
            // ���ʂ̕����������[���h���W�n�ɕϊ�
            const GSmatrix4& local_to_world_matrix = transform_.localToWorldMatrix();
            gsPlaneTransform(plane, plane, &local_to_world_matrix);
        }
        return true;
    }
    return false;
}

// ���̂Ƃ̏Փ˔���
bool FieldActor::collide(const BoundingSphere& sphere, GSvector3* collided_center) const {
    // ���[�J�����W�n�̋��̂����߂�
    BoundingSphere local_sphere;
    local_sphere.center = transform_.inverseTransformPoint(sphere.center);
    local_sphere.radius = sphere.radius;
    // ���b�V���Ƌ��̂��Փ˂������H
    GSvector3 local_collided_center;
    if (gsCollisionMeshAndSphere(gsGetMesh(mesh_collider_), &local_sphere.center, local_sphere.radius, &local_collided_center) == GS_TRUE) {
        if (collided_center != nullptr) {
            // �Փ˔����̒��S���W�����[���h���W�n�ɕϊ�
            *collided_center = transform_.transformPoint(local_collided_center);
        }
        return true;
    }
    return false;
}

void FieldActor::gravity_update(float delta_time) {
    //�n��ɂ��Ă����疳��
    if (is_ground_)return;
    if (is_zero_gravity_)return;
    GSvector3 planet_position{ 0.0f,-20.0f,0.0f };//���̒��S
    GSvector3 position = transform_.position();//�����̈ʒu
    GSvector3 gravity = position - planet_position;//�����x�N�g�������߂�
    gravity = gravity.normalize();//�P��x�N�g��
    gravity_velocity_ += gravity * gravity_ * delta_time;
    transform_.translate(gravity_velocity_, GStransform::Space::World);//�ړ�

}
void FieldActor::collide_ground() {
    // �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
    GSvector3 line_start = transform_.position() + transform_.up() * height_;
    GSvector3 down_direction = -transform_.up();
    GSvector3 line_end = transform_.position() + (down_direction * foot_offset_);
    GSvector3 collision_point;    // �Փ˂����n�ʂƂ̌�_
    GSplane ground_plane;         // �Փ˂����n�ʂ̕���
    if (gsOctreeCollisionLine(gsGetOctree(Octree_TestStageCollider),
        &line_start, &line_end, &collision_point, &ground_plane)) {
        // �Փ˂����ʒu�ɍ��W��␳����
        transform_.position(collision_point);
        //�d�͂𖳌�
        is_ground_ = true;
        gravity_velocity_ = GSvector3::zero();
        // �Ζʂɍ��킹�ăL�����N�^���X������
        GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
        GSvector3 up = transform_.position() - planet_position;
        GSvector3 left = GSvector3::cross(up, transform_.forward());
        GSvector3 forward = GSvector3::cross(left, up);
        transform_.rotation(GSquaternion::lookRotation(forward, up));
    }
    else  is_ground_ = false;

}
void FieldActor::collide_actor(Actor& other) {
    // �����W���������W�����߂�
    GSvector3 position = transform_.position();
    GSvector3 target = other.transform().position();
    // ����Ƃ̋���
    float distance = GSvector3::distance(position, target);
    // �Փ˔��苅�̔��a���m�����������������߂�
    float length = collider_.radius + other.collider().radius;
    // �Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
    float overlap = length - distance;
    // �d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
    GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
    transform_.translate(v, GStransform::Space::World);
}

