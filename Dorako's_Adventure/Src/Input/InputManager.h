#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include "InputMap.h"

enum class InputDeviceType {
    Keyboard,
    GamePad,
};

// 入力を司るクラス
// InputMapのまとめ役
// 静的クラスかつ実体を持つことができないことに注意
class InputManager {
private:    // 実体を持つことを禁止する
    // デフォルトコンストラクタ
    InputManager() = default;
    // デストラクタ
    ~InputManager() = default;

    // コピー禁止
    InputManager(const InputManager& other) = delete;
    InputManager& operator = (const InputManager& other) = delete;

public:
    // 初期化処理
    static void initialize();
    // csvを用いた初期化処理
    static void initialize(const std::string& file_path);
    // 更新
    static void update(float delta_time);
    // 終了処理
    static void fainalize();

    // 最後に入力されたデバイスの種類を取得
    static InputDeviceType get_last_device_type();
private:
    // 最後に入力されたデバイスをチェックする
    static void last_input_device_check();

    // キーボード入力があるかを取得する
    static bool is_inputted_keyboard();
    // マウスボタン入力があるかを調べる
    static bool is_inputted_mouse_button();
    // ゲームパッドのボタン入力があるかを調べる
    static bool is_inputted_pad_button();

public:
    // ゲーム全体で共通する入力をまとめる変数
    static InputMap common_;
    // プレイヤー1に関係する入力をまとめる変数
    static InputMap player_1_;
    // プレイヤー2に関係する入力をまとめる変数
    static InputMap player_2_;
    // ゲーム中のメニュー操作に関係する入力をまとめる変数
    static InputMap menu_;

private:
    // 最後に入力されたデバイスの種類
    static InputDeviceType last_device_type_;
};

#endif // !INPUT_MANAGER_H_
