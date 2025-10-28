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
    void gravity_update(float delta_time);
    void collide_ground();
    void collide_actor(Actor& actor);

    // ���b�V��
    GSuint          mesh_{ 0 };
    // ���b�V���R���C�_
    GSuint          mesh_collider_{ 0 };

    //�d�͗�
    float gravity_{ -0.003 };
    //�d�͂ɂ��ړ���
    GSvector3 gravity_velocity_{ 0.0f,0.0f,0.0f };
    int max_health_{ 1 };
    int health_{ 1 };
    //�����̃I�t�Z�b�g
    float foot_offset_{ 0.1f };
    //�L�����̍���
    float height_{ 1.0f };
    //�d�͂�K�����邩
    bool is_ground_{ false };
    bool is_zero_gravity_{ false };

};

#endif

