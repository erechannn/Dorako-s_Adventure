#ifndef FIELD_ACTOR_MANAGER_H_
#define FIELD_ACTOR_MANAGER_H_

#include "../ActorManager.h"

class FieldActor;
class Ray;
class Line;
class BoundingSphere;
struct GSvector3;
struct GSplane;

// �t�B�[���h�A�N�^�[�}�l�[�W���[
class FieldActorManager : public ActorManager {
public:
    // ���C�Ƃ̏Փ˔���
    FieldActor* collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // �����Ƃ̏Փ˔���
    FieldActor* collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // ���̂Ƃ̏Փ˔���
    FieldActor* collide(
        const BoundingSphere& sphere,
        GSvector3* collided_center = nullptr) const;
};

#endif

