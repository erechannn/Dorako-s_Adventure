#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>
#include "../Actor/FieldActor/FieldActorManager.h"

class Ray;
class Line;
class BoundingSphere;

// �t�B�[���h�N���X
class Field {
public:
    // �R���X�g���N�^
    Field(GSuint octree, GSuint collider, GSuint skybox);
    // �f�X�g���N�^
    ~Field();
    // �X�V
    void update(float delta_time);
    // �`��
    void draw() const;
    // �t�B�[���h�p�A�N�^�[�̕`��
    void draw_actors() const;

    // ���C�Ƃ̏Փ˔���
    bool collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr,
        Actor** actor = nullptr) const;
    // �����Ƃ̏Փ˔���
    bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr,
        Actor** actor = nullptr) const;
    // ���̂Ƃ̏Փ˔���
    bool collide(
        const BoundingSphere& sphere,
        GSvector3* center = nullptr,
        Actor** actor = nullptr) const;

    // �t�B�[���h�p�A�N�^�[�̒ǉ�
    void add(Actor* actor);
    // �t�B�[���h�p�A�N�^�[�̏���
    void clear();

    // �R�s�[�֎~
    Field(const Field& other) = delete;
    Field& operator = (const Field& other) = delete;

private:
    // �`��p�I�N�c���[
    GSuint  octree_;
    // �Փ˔��蔻��p�I�N�c���[
    GSuint  collider_;
    // �X�J�C�{�b�N�X
    GSuint  skybox_;
    // �t�B�[���h�A�N�^�[�}�l�[�W���[
    FieldActorManager actors_;
};

#endif

