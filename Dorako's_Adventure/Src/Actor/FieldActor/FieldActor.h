#ifndef FIELD_ACTOR_H_
#define FIELD_ACTOR_H_

#include "../Actor.h"

// �O���錾
class Ray;
class Line;
class BoundingSphere;

// �t�B�[���h�p�̃A�N�^�[�N���X
class FieldActor : public Actor {
public:
    // �`��
    virtual void draw() const override;

    // ���C�Ƃ̏Փ˔���
    virtual bool collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // �����Ƃ̏Փ˔���
    virtual bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // ���̂Ƃ̏Փ˔���
    virtual bool collide(
        const BoundingSphere& sphere,
        GSvector3* intersect = nullptr) const;

protected:
    // ���b�V��
    GSuint          mesh_{ 0 };
    // ���b�V���R���C�_
    GSuint          mesh_collider_{ 0 };
};

#endif

