#include "GamePadInput.h"

static const GSushort BoolCode[PadBoolCount]{
    {GS_XBOX_PAD_UP},
    {GS_XBOX_PAD_DOWN},
    {GS_XBOX_PAD_LEFT},
    {GS_XBOX_PAD_RIGHT},
    {GS_XBOX_PAD_START},
    {GS_XBOX_PAD_BACK},
    {GS_XBOX_PAD_LEFT_THUMB},
    {GS_XBOX_PAD_RIGHT_THUMB},
    {GS_XBOX_PAD_LEFT_SHOULDER},
    {GS_XBOX_PAD_RIGHT_SHOULDER},
    {GS_XBOX_PAD_A},
    {GS_XBOX_PAD_B},
    {GS_XBOX_PAD_X},
    {GS_XBOX_PAD_Y},
};

// ボタンが押され始めたか？
bool GamePadInput::is_started(PadBool bool_code, size_t pad_number) {
    return gsXBoxPadButtonTrigger(pad_number, BoolCode[(size_t)bool_code]);
}
// ボタンが押されている最中か？
bool GamePadInput::is_pressed(PadBool bool_code, size_t pad_number) {
    return gsXBoxPadButtonState(pad_number, BoolCode[(size_t)bool_code]);
}
// ボタンが離されたか？
bool GamePadInput::is_canceled(PadBool bool_code, size_t pad_number) {
    return gsXBoxPadButtonDetach(pad_number, BoolCode[(size_t)bool_code]);
}

// スティックの入力を取得
GSvector2 GamePadInput::get_stick(PadVec2 vec2_code, size_t pad_number) {
    GSvector2 input{};
    switch (vec2_code) {
    case PadVec2::LeftStick: {
        gsXBoxPadGetLeftAxis(pad_number, &input);
        input = GSvector2::clampMagnitude(input, 1);
        break;
    }
    case PadVec2::RightStick: {
        gsXBoxPadGetRightAxis(pad_number, &input);
        input = GSvector2::clampMagnitude(input, 1);
        break;
    }
    default: {
        break;
    }
    }
    return input;
}
// トリガーの入力を取得
GSfloat GamePadInput::get_trigger(PadFloat float_code, size_t pad_number) {
    float input{};
    switch (float_code) {
    case PadFloat::LeftTrigger: {
        input = gsXBoxPadGetLeftTrigger(pad_number);
        break;
    }
    case PadFloat::RightTrigger: {
        input = gsXBoxPadGetRightTrigger(pad_number);
        break;
    }
    default: {
        break;
    }
    }
    return input;
}
