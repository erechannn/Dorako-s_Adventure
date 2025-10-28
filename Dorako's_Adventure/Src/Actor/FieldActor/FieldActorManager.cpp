#include "FieldActorManager.h"
#include "FieldActor.h"
#include "../../Shape/Line.h"
#include "../../Shape/Ray.h"
#include "../../Collider/BoundingSphere.h"
#include <limits> // numeric_limits���g���̂ɕK�v

// ���C�ƃt�B�[���h�p�̃A�N�^�[�Ƃ̏Փ˔���
FieldActor* FieldActorManager::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane) const {
    // ���C������ɕϊ�����
    Line line;
    line.start = ray.position;
    line.end = ray.position + (ray.direction.normalized() * max_distance);
    // �����Ƃ̔�����s��
    return collide(line, intersect, plane);
}

// �����ƃt�B�[���h�p�̃A�N�^�[�Ƃ̏Փ˔���
FieldActor* FieldActorManager::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
    // �ł��n�_����߂��A�N�^�[
    FieldActor* closest_actor{ nullptr };
    // �ł��n�_����߂���_�Ƃ̋���
    float closest_distance = std::numeric_limits<float>::max(); // float�^�̍ő�l
    // �ł��n�_����߂���_
    GSvector3 closest_intersection;
    // �ł��n�_����߂���_�̕���
    GSplane closest_intersection_plane;

    for (auto actor : actors_) {
        // �t�B�[���h�A�N�^�[�ł��邩�m�F
        FieldActor* field_actor = dynamic_cast<FieldActor*>(actor);
        // �t�B�[���h�A�N�^�[�łȂ���΃X�L�b�v
        if (field_actor == nullptr) continue;
        // �t�B�[���h�A�N�^�[�Ɛ����Ƃ̌���������s��
        GSvector3 intersection_point;
        GSplane intersection_plane;
        if (field_actor->collide(line, &intersection_point, &intersection_plane)) {
            // �n�_����̋������v�Z
            float distance = line.start.distance(intersection_point);
            if (distance < closest_distance) {
                // �������߂��ꍇ�́A�A�N�^�[���X�V
                closest_distance = distance;
                closest_actor = field_actor;
                closest_intersection = intersection_point;
                closest_intersection_plane = intersection_plane;
            }
        }
    }
    // �Փ˂����A�N�^�[�����݂��邩�H
    if (closest_actor != nullptr) {
        if (intersect != nullptr) {
            *intersect = closest_intersection;
        }
        if (plane != nullptr) {
            *plane = closest_intersection_plane;
        }
    }
    return closest_actor; // �n�_�ɍł��߂��A�N�^�[��Ԃ�
}

// ���̂ƃt�B�[���h�p�̃A�N�^�[�Ƃ̏Փ˔���
FieldActor* FieldActorManager::collide(const BoundingSphere& sphere, GSvector3* collided_center) const {
    for (auto actor : actors_) {
        // �t�B�[���h�A�N�^�[�ł��邩�m�F
        FieldActor* field_actor = dynamic_cast<FieldActor*>(actor);
        // �t�B�[���h�A�N�^�[�łȂ���΃X�L�b�v
        if (field_actor == nullptr) continue;
        // �t�B�[���h�A�N�^�[�Ƌ��̂̏Փ˔�����s��
        if (field_actor->collide(sphere, collided_center)) {
            return field_actor;
        }
    }
    return nullptr;
}

