#pragma once
#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <vector>
#include <functional>
#include <memory>

struct animation_event
{
    animation_event(GSuint motion, GSfloat time, std::function<void()>callback)
        :motion_{ motion }, time_{ time }, callback_{ callback }
    {
    }

    GSuint motion_;
    GSfloat time_;
    std::function<void()>callback_;
};

// �A�j���[�V�����t�����b�V���N���X
class AnimatedMesh {
public:
    // �R���X�g���N�^
    AnimatedMesh(GSuint id, GSuint motion = 0, bool loop = true, GSuint num_bones = 256);
    // �X�V
    void update(float delta_time);
    // �`��
    void draw() const;
    // �`��
    void draw(const GSmatrix4& mat) const;
    // ���[�V�����ύX
    void change_motion(GSuint motion, bool loop = true);
    // �ϊ��s���ݒ肷��
    void transform(const GSmatrix4& matrix);
    // ���[�V�������I�����Ă��邩�H
    bool is_end_motion() const;
    // ���݂̃��[�V�����̍Đ����Ԃ��擾
    float motion_time() const;
    // ���݂̃��[�V�����Đ����Ԃ�ݒ�
    void motion_time(float time);
    // ���[�V�����̏I�����Ԃ��擾
    float motion_end_time() const;
    // �{�[���̃��[���h�ϊ��s����擾
    GSmatrix4 bone_matrices(int bone_no) const;

    void add_event(GSuint motion, GSfloat time, std::function<void()>callback);

private:
    // �A�Z�b�gID
    GSuint id_;
    // ���[�V�����ԍ�
    GSuint motion_;
    // ���[�V�����^�C�}�[
    GSfloat motion_timer_;
    // ���[�V�������[�v�t���O
    bool motion_loop_;
    // ���[���h�ϊ��s��
    GSmatrix4 transform_;
    // �O��Đ��������[�V�����ԍ�
    GSuint prev_motion_;
    // �O��Đ������ŏI�A�j���[�V�����^�C�}�[
    GSfloat prev_motion_timer_;
    // ��ԃ^�C�}�[
    GSfloat lerp_timer_;

    // �{�[���̃��[�J���p�ϊ��s��
    std::vector<GSmatrix4> local_bone_matrices_;
    // �{�[���̕ϊ��s��
    std::vector<GSmatrix4> bone_matrices_;

    std::vector<std::unique_ptr<animation_event>>events_;
};

#endif // !ANIMATED_MESH_H_
