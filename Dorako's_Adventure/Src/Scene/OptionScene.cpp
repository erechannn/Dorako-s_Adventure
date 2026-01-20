#include "OptionScene.h"
#include "../Assets.h"

#include <imgui/imgui.h>

OptionScene::OptionScene() {
	initialize();
}
void OptionScene::initialize() {
	flash_time_ = 0.0f;
	volume_max_ = 1.0f;
	is_end_ = false;
	is_camera_option_ = true;
	sound_option_ = true;
	select_ = false;
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

	flash_time_ += delta_time;

	//if (ImGui::Begin("Option_ui")) {
	//	ImGui::DragFloat2("UI_position", position_, 0.1f);
	//	ImGui::DragFloat2("UI_size", size_, 0.1f);
	//	ImGui::DragFloat4("UI_rect", rect_, 0.1f);
	//	ImGui::End();
	//}
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
		else {
			option_ = Option::SoundOption;
		}
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B)) {
		is_end_ = true;
	}
}
void OptionScene::sound_option_update() {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_DOWN)) {
		sound_option_ = false;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_UP)) {
		sound_option_ = true;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
		select_ = true;
	}
	if (select_) {
		if (sound_option_) {
			if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT)) {
				volume_BGM_ += 0.1f;
				if (volume_BGM_ >= 1.0f)volume_BGM_ = 1.0f;
			}
			if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT)) {
				volume_BGM_ -= 0.1f;
				if (volume_BGM_ <= 0.0f)volume_BGM_ = 0.0f;
			}
			gsSetVolumeBGM(volume_BGM_);
		}
		else if (!sound_option_) {
			if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT)) {
				volume_SE_ += 0.1f;
				if (volume_SE_ >= 1.0f)volume_SE_ = 1.0f;
			}
			if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT)) {
				volume_SE_ -= 0.1f;
				if (volume_SE_ <= 0.0f)volume_SE_ = 0.0f;
			}
			gsSetMasterVolumeSE(volume_SE_);

		}
	}

	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B)&&!select_) {
		option_ = Option::NullSelect;
		select_ = false;
	}
	else if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B)) {
		select_ = false;
	}

}
void OptionScene::camera_option_update() {

	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_B)) {
		option_ = Option::NullSelect;
	}

}
void OptionScene::draw()const {
	draw_back_ground();
	if (option_ == Option::CameraOption) {
		camera_option_draw();
	}
	else if (option_ == Option::SoundOption) {
		sound_option_draw();
	}
	else draw_null_select();
}
void OptionScene::draw_back_ground()const {
	GSvector2 position{ 500.0f,200.0f };
	GSvector2 size{ 0.5f,0.5f };

	gsDrawSprite2D(Texture_BackGround, &position, NULL, NULL, NULL, &size, 0.0f);

}
void OptionScene::draw_null_select()const {
	GSrect option_rect{ 0.0f,128.0f,700.0f,256.0f };
	GSrect camera_rect{ 0.0f,128.0f,700.0f,256.0f };
	GSrect sound_rect{ 0.0f,0.0f,700.0f,128.0f };
	GSvector2 option_position{ 740.0f,200.0f };
	GSvector2 camera_position{ 760.0f,410.0f };
	GSvector2 sound_position{ 720.0f,560.0f };

	GSvector2 size{ 0.8f,0.8f };

	GScolor camera_color{ 1.0f,1.0f,1.0f,1.0f };
	GScolor sound_color{ 1.0f,1.0f,1.0f,1.0f };
	if (is_camera_option_ && std::fmod(flash_time_, 40.0f) <= 10.0f)camera_color = { 1.0f,1.0f,1.0f,0.5f };
	if (!is_camera_option_ && std::fmod(flash_time_, 40.0f) <= 10.0f)sound_color = { 1.0f,1.0f,1.0f,0.5f };

	gsDrawSprite2D(Texture_Pause, &option_position, &option_rect, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_Option, &camera_position, &camera_rect, NULL, &camera_color, &size, 0.0f);
	gsDrawSprite2D(Texture_Option, &sound_position, &sound_rect, NULL, &sound_color, &size, 0.0f);
}
void OptionScene::sound_option_draw()const {
	GSvector2 size{ 0.8f,0.8f };
	GSrect sound_ui_rect{ 0.0f,0.0f,700.0f,128.0f };
	GSrect BGM_rect{ 0.0f,256.0f,250.0f,384.0f };
	GSrect SE_rect{ 0.0f,384.0f,200.0f,512.0f };

	GSvector2 sound_ui_position{ 650.0f,220.0f };
	GSvector2 BGM_position{ 570.0f,400.0f };
	GSvector2 SE_position{ 570.0f,560.0f };
	GSvector2 BGM_gauge_position{ 800.0f,430.0f };
	GSvector2 SE_gauge_position{ 800.0f,590.0f };

	GScolor BGM_color{ 1.0f,1.0f,1.0f,1.0f };
	GScolor SE_color{ 1.0f,1.0f,1.0f,1.0f };
	if (sound_option_ && std::fmod(flash_time_, 40.0f) <= 10.0f)BGM_color = { 1.0f,1.0f,1.0f,0.5f };
	if (!sound_option_ && std::fmod(flash_time_, 40.0f) <= 10.0f)SE_color = { 1.0f,1.0f,1.0f,0.5f };


	gsDrawSprite2D(Texture_Option, &sound_ui_position, &sound_ui_rect, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_Option, &BGM_position, &BGM_rect, NULL, &BGM_color, &size, 0.0f);
	gsDrawSprite2D(Texture_Option, &SE_position, &SE_rect, NULL, &SE_color, &size, 0.0f);

	gsDrawSprite2D(Texture_SoundGaugeEmpty, &BGM_gauge_position, NULL, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_SoundGaugeEmpty, &SE_gauge_position, NULL, NULL, NULL, NULL, 0.0f);

	float BGM_gauge_left = (600 / volume_max_) * volume_BGM_;
	float SE_gauge_left = (600 / volume_max_) * volume_SE_;

	GSrect BGM_gauge_rect{ 0.0f,0.0f,BGM_gauge_left,46.0f };
	GSrect SE_gauge_rect{ 0.0f,0.0f,SE_gauge_left,46.0f };
	gsDrawSprite2D(Texture_SoundGauge, &BGM_gauge_position, &BGM_gauge_rect, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_SoundGauge, &SE_gauge_position, &SE_gauge_rect, NULL, NULL, NULL, 0.0f);

}
void OptionScene::camera_option_draw()const {
	GSvector2 position{ 480.0f,300.0f };
	gsDrawSprite2D(Texture_kaihatu, &position_, NULL, NULL, NULL, NULL, 0.0f);
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
