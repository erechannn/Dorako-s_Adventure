#ifndef INPUT_MAP_H_
#define INPUT_MAP_H_

#include "InputAction.h"

// 入力の大まかな種別を表すクラス
// InputActionのまとめ役
class InputMap {
public:
    // デフォルトコンストラクタ
    InputMap() = default;

    // 更新
    void update(float delta_time);

    // 入力を有効化
    void enable();
    // 入力を無効化
    void disable();
    // 入力が有効になっているか?
    bool is_active() const;

    // インプットアクションの初期化
    void clear();

    // 名前からインプットアクションの参照を取得
    const InputAction& operator [](std::string name) const;
    // 名前からインプットアクションの参照を取得
    InputAction& operator [](std::string name);

    // コピー禁止
    InputMap(const InputMap& other) = delete;
    InputMap& operator = (const InputMap& other) = delete;
private:
    // インプットアクションのマップ
    std::map<std::string, InputAction> actions_{};
    // 入力が有効になっているか?
    bool is_active_{ false };
};

#endif // !INPUT_MAP_H_
