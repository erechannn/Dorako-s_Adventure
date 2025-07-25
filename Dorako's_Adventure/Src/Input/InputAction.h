#ifndef INPUT_ACTION_H_
#define INPUT_ACTION_H_

#include <vector>

using Keycode = DWORD;
using PadNumber = unsigned int;
using PadButton = unsigned short;
using MouseButton = GMOUSTATE;

// 1つのアクションに対応する入力を設定するクラス
class InputAction {
public:
    // デフォルトコンストラクタ
    InputAction() = default;
    // デストラクタ
    ~InputAction() = default;
    // 更新
    void update(float delta_time);

    // 入力を有効化
    void enable();
    // 入力を無効化
    void disable();

    // 入力をチェックするキーボードのコードを追加する
    void add_keycode(Keycode code);
    // 入力をチェックするパッドのボタンコードを追加する
    void add_pad_button(PadButton code, PadNumber pad_num = 0);
    // 入力をチェックするマウスのボタンコードを追加する
    void add_mouse_button(MouseButton code);

    // 同時押し入力をチェックするキーボードのコードを追加する
    void add_keycode2(Keycode code1, Keycode code2);
    // 同時押し入力をチェックするパッドのボタンコードを追加する
    void add_pad_button2(PadButton code1, PadButton code2, PadNumber pad_num = 0);
    // 同時押し入力をチェックするマウスのボタンコードを追加する
    void add_mouse_button2(MouseButton code1, MouseButton code2);

    // 入力を強制的に偽に変換するキーボードのコードを追加する
    void add_ex_keycode(Keycode code);
    // 入力を強制的に偽に変換するパッドのボタンコードを追加する
    void add_ex_pad_button(PadButton code, PadNumber pad_num = 0);
    // 入力を強制的に偽に変換するマウスのボタンコードを追加する
    void add_ex_mouse_button(MouseButton code);

    // 登録されているコードを初期化する
    void clear_all();

    // 入力が有効になっているか?
    bool is_enabled() const;

    // 入力が始まったか?
    bool started() const;
    // 入力が続いているか?
    bool pressed() const;
    // 入力が終了したか
    bool canceled() const;

    // コピー禁止
    InputAction(const InputAction& other) = delete;
    InputAction& operator = (const InputAction& other) = delete;
private:
    // 入力をチェックするコードの配列
    std::vector<Keycode> keyboard_code_{};
    std::vector<std::pair<PadButton, PadNumber>> pad_code_{};
    std::vector<MouseButton> mouse_code_{};

    // 同時押し入力をチェックするコードの配列
    std::vector<std::pair<Keycode, Keycode>> keyboard_code_pair_{};
    std::vector<std::tuple<PadButton, PadButton, PadNumber>> pad_code_pair_{};
    std::vector<std::pair<MouseButton, MouseButton>> mouse_code_pair_{};

    // 入力を強制的に偽に変えるコードの配列(ここでは排他入力コードと呼ぶ)
    std::vector<Keycode> exclusive_keyboard_code_{};
    std::vector<std::pair<PadButton, PadNumber>> exclusive_pad_code_{};
    std::vector<MouseButton> exclusive_mouse_code_{};

    bool is_prev_pressed_{ false };
    bool is_pressed_{ false };

    bool is_active_{ false };
};

#endif // !INPUT_ACTION_H_

