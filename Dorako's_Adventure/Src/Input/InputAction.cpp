#include "InputAction.h"

#include <gslib.h>

// 更新
void InputAction::update(float delta_time) {
    is_prev_pressed_ = is_pressed_;
    is_pressed_ = false;

    bool exclusive_pressed{ false };
    // 排他入力のチェック(キーボード)
    for (const auto& code : exclusive_keyboard_code_) {
        if (exclusive_pressed) break;
        exclusive_pressed = gsGetKeyState(code);
    }
    // 排他入力のチェック(パッド)
    for (const auto& code : exclusive_pad_code_) {
        if (exclusive_pressed) break;
        exclusive_pressed = gsXBoxPadButtonState(code.second, code.first);
    }
    // 排他入力のチェック(マウス)
    for (const auto& code : exclusive_mouse_code_) {
        if (exclusive_pressed) break;
        exclusive_pressed = gsGetMouseButtonState(code);
    }

    // 排他入力がない場合、入力チェックを行う
    if (!exclusive_pressed) {
        bool pressed_now{ false };
        // キーボード入力チェック
        for (const auto& code : keyboard_code_) {
            if (pressed_now) break;
            pressed_now = gsGetKeyState(code);
        }
        // パッド入力チェック
        for (const auto& code : pad_code_) {
            if (pressed_now) break;
            pressed_now = gsXBoxPadButtonState(code.second, code.first);
        }
        // マウス入力チェック
        for (const auto& code : mouse_code_) {
            if (pressed_now) break;
            pressed_now = gsGetMouseButtonState(code);
        }
        // キーボード同時押し入力チェック
        for (const auto& code : keyboard_code_pair_) {
            if (pressed_now) break;
            pressed_now = gsGetKeyState(code.first) && gsGetKeyState(code.second);
        }
        // パッド同時押し入力チェック
        for (const auto& code : pad_code_pair_) {
            if (pressed_now) break;
            auto& [code1, code2, num] = code;
            pressed_now = gsXBoxPadButtonState(num, code1) && gsXBoxPadButtonState(num, code2);
        }
        // マウス同時押し入力チェック
        for (const auto& code : mouse_code_pair_) {
            if (pressed_now) break;
            pressed_now = gsGetMouseButtonState(code.first) && gsGetMouseButtonState(code.second);
        }
        is_pressed_ = pressed_now;
    }
}

// 入力を有効化
void InputAction::enable() {
    is_active_ = true;
}
// 入力を無効化
void InputAction::disable() {
    is_active_ = false;
}

// 入力をチェックするキーボードのコードを追加する
void InputAction::add_keycode(Keycode code) {
    keyboard_code_.push_back(code);
}
// 入力をチェックするパッドのボタンコードを追加する
void InputAction::add_pad_button(PadButton code, PadNumber pad_num) {
    pad_code_.push_back({ code, pad_num });
}
// 入力をチェックするマウスのボタンコードを追加する
void InputAction::add_mouse_button(MouseButton code) {
    mouse_code_.push_back(code);
}

// 同時押し入力をチェックするキーボードのコードを追加する
void InputAction::add_keycode2(Keycode code1, Keycode code2) {
    keyboard_code_pair_.push_back({ code1, code2 });
}
// 同時押し入力をチェックするパッドのボタンコードを追加する
void InputAction::add_pad_button2(PadButton code1, PadButton code2, PadNumber pad_num) {
    pad_code_pair_.push_back({ code1, code2, pad_num });
}
// 同時押し入力をチェックするマウスのボタンコードを追加する
void InputAction::add_mouse_button2(MouseButton code1, MouseButton code2) {
    mouse_code_pair_.push_back({ code1, code2 });
}

// 入力を強制的に偽に変換するキーボードのコードを追加する
void InputAction::add_ex_keycode(Keycode code) {
    exclusive_keyboard_code_.push_back(code);
}
// 入力を強制的に偽に変換するパッドのボタンコードを追加する
void InputAction::add_ex_pad_button(PadButton code, PadNumber pad_num) {
    exclusive_pad_code_.push_back({ code, pad_num });
}
// 入力を強制的に偽に変換するマウスのボタンコードを追加する
void InputAction::add_ex_mouse_button(MouseButton code) {
    exclusive_mouse_code_.push_back(code);
}

// 登録されているコードをリセットする
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

// 入力が有効になっているか?
bool InputAction::is_enabled() const {
    return is_active_;
}

// 入力が始まったか?
bool InputAction::started() const {
    return is_pressed_ && !is_prev_pressed_ && is_active_;
}
// 入力が続いているか?
bool InputAction::pressed() const {
    return is_pressed_ && is_prev_pressed_ && is_active_;
}
// 入力が終了したか
bool InputAction::canceled() const {
    return !is_pressed_ && is_prev_pressed_ && is_active_;
}