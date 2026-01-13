#include "OptionScene.h"

#include <imgui/imgui.h>

OptionScene::OptionScene() {
	initialize();
}
void OptionScene::initialize() {
	is_end_ = false;
	is_camera_option_ = true;
	volume_BGM_ = gsGetVolumeBGM();
	volume_SE_ = gsGetMasterVolumeSE();
	option_ = Option::NullSelect;
}
void OptionScene::update(float delta_time) {
	switch (option_) {
	case Option::NullSelect:null_select_update(); break;
	case Option::CameraOption:camera_option_update(); break;
	case Option::SoundOption:sound_option_update(); break;	
	}

	if (ImGui::Begin("Option_ui")) {
		ImGui::DragFloat2("UI_position", position_, 0.1f);
		ImGui::DragFloat4("UI_rect", rect_, 0.1f);
		ImGui::End();

	}
}
void OptionScene::null_select_update() {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_DOWN) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_UP) ){
		if (is_camera_option_) {
			is_camera_option_=false;
		}
		else is_camera_option_=true;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
		if (is_camera_option_) {
			option_ = Option::CameraOption;
		}
		else option_ = Option::SoundOption;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B)) {
		is_end_ = true;
	}
}
void OptionScene::sound_option_update() {

}
void OptionScene::camera_option_update() {

}
void OptionScene::draw()const {

}
void OptionScene::sound_option_draw()const {

}
void OptionScene::camera_option_draw()const {

}
bool OptionScene::is_end() {
	return is_end_;
}
bool OptionScene::is_reverse_camera_x() {
	return is_reverse_camera_x_;
}
bool OptionScene::is_reverse_camera_y() {
	return is_reverse_camera_y_;
}
