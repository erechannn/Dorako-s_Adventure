#ifndef GAMEPAD_INPUT_H_
#define GAMEPAD_INPUT_H_

enum class PadVec2 {
    LeftStick,
    RightStick
};
constexpr size_t PadVec2Count = 2;

enum class PadFloat {
    LeftTrigger,
    RightTrigger,
};
constexpr size_t PadFloatCount = 2;

enum class PadBool {
    Up, Down, Left, Right,
    Start, Back,
    LeftStick, RightStick,
    L1, R1,
    A, B, X, Y
};
constexpr size_t PadBoolCount = 14;

class GamePadInput {
private:    // 実体を持つことを禁止する
    // デフォルトコンストラクタ
    GamePadInput() = default;
    // デストラクタ
    ~GamePadInput() = default;

    // コピー禁止
    GamePadInput(const GamePadInput& other) = delete;
    GamePadInput& operator = (const GamePadInput& other) = delete;

public:
    // ボタンが押され始めたか？
    static bool is_started(PadBool bool_code, size_t pad_number);
    // ボタンが押されている最中か？
    static bool is_pressed(PadBool bool_code, size_t pad_number);
    // ボタンが離されたか？
    static bool is_canceled(PadBool bool_code, size_t pad_number);

    // スティックの入力を取得
    static GSvector2 get_stick(PadVec2 vec2_code, size_t pad_number);
    // トリガーの入力を取得
    static GSfloat get_trigger(PadFloat float_code, size_t pad_number);

};

#endif // !GAMEPAD_INPUT_H_
