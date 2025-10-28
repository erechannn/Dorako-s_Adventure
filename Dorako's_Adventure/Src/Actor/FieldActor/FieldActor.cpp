#include "FieldActor.h"
#include "../../Shape/Line.h"
#include "../../Shape/Ray.h"
#include "../../Collider/BoundingSphere.h"

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

