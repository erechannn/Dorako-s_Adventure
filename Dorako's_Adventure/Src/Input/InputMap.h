#ifndef INPUT_MAP_H_
#define INPUT_MAP_H_

#include "InputAction.h"

// ���͂̑�܂��Ȏ�ʂ�\���N���X
// InputAction�̂܂Ƃߖ�
class InputMap {
public:
    // �f�t�H���g�R���X�g���N�^
    InputMap() = default;

    // �X�V
    void update(float delta_time);

    // ���͂�L����
    void enable();
    // ���͂𖳌���
    void disable();
    // ���͂��L���ɂȂ��Ă��邩?
    bool is_active() const;

    // �C���v�b�g�A�N�V�����̏�����
    void clear();

    // ���O����C���v�b�g�A�N�V�����̎Q�Ƃ��擾
    const InputAction& operator [](std::string name) const;
    // ���O����C���v�b�g�A�N�V�����̎Q�Ƃ��擾
    InputAction& operator [](std::string name);

    // �R�s�[�֎~
    InputMap(const InputMap& other) = delete;
    InputMap& operator = (const InputMap& other) = delete;
private:
    // �C���v�b�g�A�N�V�����̃}�b�v
    std::map<std::string, InputAction> actions_{};
    // ���͂��L���ɂȂ��Ă��邩?
    bool is_active_{ false };
};

#endif // !INPUT_MAP_H_
