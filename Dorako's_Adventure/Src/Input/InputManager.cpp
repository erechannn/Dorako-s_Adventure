#include "InputManager.h"

// static変数初期化部
InputMap InputManager::common_{};
InputMap InputManager::player_1_{};
InputMap InputManager::player_2_{};
InputMap InputManager::menu_{};

InputDeviceType InputManager::last_device_type_{ InputDeviceType::Keyboard };

// 初期化処理
void InputManager::initialize() {
	// mapの初期化
	common_.clear();
	player_1_.clear();
	player_2_.clear();
	menu_.clear();

	// ゲーム全体の操作を定義
	common_["Submit"].add_keycode(GKEY_SPACE);
	common_["Cancel"].add_keycode(GKEY_X);
	common_["MoveLeft"].add_keycode(GKEY_A);
	common_["MoveLeft"].add_keycode(GKEY_LEFT);
	common_["MoveUp"].add_keycode(GKEY_W);
	common_["MoveUp"].add_keycode(GKEY_UP);
	common_["MoveRight"].add_keycode(GKEY_D);
	common_["MoveRight"].add_keycode(GKEY_RIGHT);
	common_["MoveDown"].add_keycode(GKEY_S);
	common_["MoveDown"].add_keycode(GKEY_DOWN);
	common_["Submit"].add_pad_button(GS_XBOX_PAD_A);
	common_["Cancel"].add_pad_button(GS_XBOX_PAD_B);
	common_["MoveLeft"].add_pad_button(GS_XBOX_PAD_LEFT);
	common_["MoveUp"].add_pad_button(GS_XBOX_PAD_UP);
	common_["MoveRight"].add_pad_button(GS_XBOX_PAD_RIGHT);
	common_["MoveDown"].add_pad_button(GS_XBOX_PAD_DOWN);
}

// csvを用いた初期化処理
void InputManager::initialize(const std::string& file_path) {

}

// 更新
void InputManager::update(float delta_time) {
	common_.update(delta_time);
	player_1_.update(delta_time);
	player_2_.update(delta_time);
	menu_.update(delta_time);
	last_input_device_check();
}
// 終了処理
void InputManager::fainalize() {

}

// 最後に入力されたデバイスの種類を取得
InputDeviceType InputManager::get_last_device_type() {
	return last_device_type_;
}

// 最後に入力されたデバイスをチェックする
void InputManager::last_input_device_check() {
	if (is_inputted_pad_button()) {
		last_device_type_ = InputDeviceType::GamePad;
		return;
	}
	if (is_inputted_mouse_button()) {
		last_device_type_ = InputDeviceType::Keyboard;
		return;
	}
	if (is_inputted_keyboard()) {
		last_device_type_ = InputDeviceType::Keyboard;
		return;
	}
}

// キーボード入力があるかを取得する
bool InputManager::is_inputted_keyboard() {
	return gsGetKey() != 0;
}

// マウスボタン入力があるかを調べる
bool InputManager::is_inputted_mouse_button() {
	if (gsGetMouseButtonState(GMOUSE_BUTTON_1)) return true;
	if (gsGetMouseButtonState(GMOUSE_BUTTON_2)) return true;
	if (gsGetMouseButtonState(GMOUSE_BUTTON_3)) return true;
	if (gsGetMouseButtonState(GMOUSE_BUTTON_4)) return true;
	return false;
}

// ゲームパッドのボタン入力があるかを調べる
bool InputManager::is_inputted_pad_button() {
	GSuint pad_count = gsXBoxGetPadCount();
	if (pad_count == 0) return false;
	for (size_t i = 0; i < pad_count; ++i) {
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_UP)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_DOWN)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_LEFT)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_RIGHT)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_START)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_BACK)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_LEFT_THUMB)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_RIGHT_THUMB)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_LEFT_SHOULDER)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_RIGHT_SHOULDER)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_A)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_B)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_X)) return true;
		if (gsXBoxPadButtonState(i, GS_XBOX_PAD_Y)) return true;
	}
	return false;
}
