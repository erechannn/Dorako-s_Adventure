#include "InputMap.h"

// 更新
void InputMap::update(float delta_time) {
    for (auto& pair : actions_) {
        pair.second.update(delta_time);
    }
}

// 入力を有効化
void InputMap::enable() {
    for (auto& pair : actions_) {
        pair.second.enable();
    }
    is_active_ = true;
}
// 入力を無効化
void InputMap::disable() {
    for (auto& pair : actions_) {
        pair.second.disable();
    }
    is_active_ = false;
}
// 入力が有効になっているか
bool InputMap::is_active() const {
    return is_active_;
}

// インプットアクションの初期化
void InputMap::clear() {
    for (auto& action : actions_) {
        action.second.clear_all();
    }
    actions_.clear();
}

// 名前からインプットアクションの参照を取得
const InputAction& InputMap::operator [](std::string name) const {
    return actions_.at(name);
}
// 名前からインプットアクションの参照を取得
InputAction& InputMap::operator [](std::string name) {
    return actions_[name];
}
