#include "InputAction.h"

#include <gslib.h>

// �X�V
void InputAction::update(float delta_time) {
    is_prev_pressed_ = is_pressed_;
    is_pressed_ = false;

    bool exclusive_pressed{ false };
    // �r�����͂̃`�F�b�N(�L�[�{�[�h)
    for (const auto& code : exclusive_keyboard_code_) {
        if (exclusive_pressed) break;
        exclusive_pressed = gsGetKeyState(code);
    }
    // �r�����͂̃`�F�b�N(�p�b�h)
    for (const auto& code : exclusive_pad_code_) {
        if (exclusive_pressed) break;
        exclusive_pressed = gsXBoxPadButtonState(code.second, code.first);
    }
    // �r�����͂̃`�F�b�N(�}�E�X)
    for (const auto& code : exclusive_mouse_code_) {
        if (exclusive_pressed) break;
        exclusive_pressed = gsGetMouseButtonState(code);
    }

    // �r�����͂��Ȃ��ꍇ�A���̓`�F�b�N���s��
    if (!exclusive_pressed) {
        bool pressed_now{ false };
        // �L�[�{�[�h���̓`�F�b�N
        for (const auto& code : keyboard_code_) {
            if (pressed_now) break;
            pressed_now = gsGetKeyState(code);
        }
        // �p�b�h���̓`�F�b�N
        for (const auto& code : pad_code_) {
            if (pressed_now) break;
            pressed_now = gsXBoxPadButtonState(code.second, code.first);
        }
        // �}�E�X���̓`�F�b�N
        for (const auto& code : mouse_code_) {
            if (pressed_now) break;
            pressed_now = gsGetMouseButtonState(code);
        }
        // �L�[�{�[�h�����������̓`�F�b�N
        for (const auto& code : keyboard_code_pair_) {
            if (pressed_now) break;
            pressed_now = gsGetKeyState(code.first) && gsGetKeyState(code.second);
        }
        // �p�b�h�����������̓`�F�b�N
        for (const auto& code : pad_code_pair_) {
            if (pressed_now) break;
            auto& [code1, code2, num] = code;
            pressed_now = gsXBoxPadButtonState(num, code1) && gsXBoxPadButtonState(num, code2);
        }
        // �}�E�X�����������̓`�F�b�N
        for (const auto& code : mouse_code_pair_) {
            if (pressed_now) break;
            pressed_now = gsGetMouseButtonState(code.first) && gsGetMouseButtonState(code.second);
        }
        is_pressed_ = pressed_now;
    }
}

// ���͂�L����
void InputAction::enable() {
    is_active_ = true;
}
// ���͂𖳌���
void InputAction::disable() {
    is_active_ = false;
}

// ���͂��`�F�b�N����L�[�{�[�h�̃R�[�h��ǉ�����
void InputAction::add_keycode(Keycode code) {
    keyboard_code_.push_back(code);
}
// ���͂��`�F�b�N����p�b�h�̃{�^���R�[�h��ǉ�����
void InputAction::add_pad_button(PadButton code, PadNumber pad_num) {
    pad_code_.push_back({ code, pad_num });
}
// ���͂��`�F�b�N����}�E�X�̃{�^���R�[�h��ǉ�����
void InputAction::add_mouse_button(MouseButton code) {
    mouse_code_.push_back(code);
}

// �����������͂��`�F�b�N����L�[�{�[�h�̃R�[�h��ǉ�����
void InputAction::add_keycode2(Keycode code1, Keycode code2) {
    keyboard_code_pair_.push_back({ code1, code2 });
}
// �����������͂��`�F�b�N����p�b�h�̃{�^���R�[�h��ǉ�����
void InputAction::add_pad_button2(PadButton code1, PadButton code2, PadNumber pad_num) {
    pad_code_pair_.push_back({ code1, code2, pad_num });
}
// �����������͂��`�F�b�N����}�E�X�̃{�^���R�[�h��ǉ�����
void InputAction::add_mouse_button2(MouseButton code1, MouseButton code2) {
    mouse_code_pair_.push_back({ code1, code2 });
}

// ���͂������I�ɋU�ɕϊ�����L�[�{�[�h�̃R�[�h��ǉ�����
void InputAction::add_ex_keycode(Keycode code) {
    exclusive_keyboard_code_.push_back(code);
}
// ���͂������I�ɋU�ɕϊ�����p�b�h�̃{�^���R�[�h��ǉ�����
void InputAction::add_ex_pad_button(PadButton code, PadNumber pad_num) {
    exclusive_pad_code_.push_back({ code, pad_num });
}
// ���͂������I�ɋU�ɕϊ�����}�E�X�̃{�^���R�[�h��ǉ�����
void InputAction::add_ex_mouse_button(MouseButton code) {
    exclusive_mouse_code_.push_back(code);
}

// �o�^����Ă���R�[�h�����Z�b�g����
void InputAction::clear_all() {
    keyboard_code_.clear();
    pad_code_.clear();
    mouse_code_.clear();
    keyboard_code_pair_.clear();
    pad_code_pair_.clear();
    mouse_code_pair_.clear();
    exclusive_keyboard_code_.clear();
    exclusive_pad_code_.clear();
    exclusive_mouse_code_.clear();
}

// ���͂��L���ɂȂ��Ă��邩?
bool InputAction::is_enabled() const {
    return is_active_;
}

// ���͂��n�܂�����?
bool InputAction::started() const {
    return is_pressed_ && !is_prev_pressed_ && is_active_;
}
// ���͂������Ă��邩?
bool InputAction::pressed() const {
    return is_pressed_ && is_prev_pressed_ && is_active_;
}
// ���͂��I��������
bool InputAction::canceled() const {
    return !is_pressed_ && is_prev_pressed_ && is_active_;
}