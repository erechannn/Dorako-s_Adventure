#include "StageSelectScene.h"
#include "../Stage/StageManager.h"
#include "../Assets.h"
#include <imgui/imgui.h>


void StageSelectScene::start() {
	gsLoadTexture(Texture_StageSelect, "Assets/Texture/StageSelectUI/StageSelect.png");
	gsLoadTexture(Texture_StageSelectSceneBackground, "Assets/Texture/StageSelectUI/Background.png");
	gsLoadTexture(Texture_StageSelectStage1, "Assets/Texture/StageSelectUI/StageSelectStage1.png");
	gsLoadTexture(Texture_StageSelectBoss, "Assets/Texture/StageSelectUI/StageSelectBoss.png");
	gsLoadTexture(Texture_ArrowIcon, "Assets/Texture/ArrowIcon.png" );


	max_stage_count_ = StageManager::get_instance().get_total_stage_count();


	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "GamePlayScene";
	stage_count_ = 1;
	arrow_icon_position_ = { 560.0f,382.0f };
}

void StageSelectScene::update(float delta_time) {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT)) {
		arrow_icon_position_ = { 1090.0f,382.0f };
		stage_count_ = 2;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT)) {
		arrow_icon_position_ = { 560.0f,382.0f };
		stage_count_ = 1;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
		if (StageManager::get_instance().is_stage_unlocked(stage_count_)) {
			is_start_ = true;
		}
	}

	if (gsGetKeyTrigger(GKEY_1)) {
		stage_count_ = 1;
		is_start_ = true;
	}
	if (gsGetKeyTrigger(GKEY_2)) {
		stage_count_ = 2;
		is_start_ = true;
	}
	if (gsGetKeyTrigger(GKEY_3)) {
		stage_count_ = 3;
		is_start_ = true;
	}


	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了

	//if (ImGui::Begin("UI_position")) {
	//	ImGui::DragFloat2("Position : ", position_, 0.1f);
	//	ImGui::DragFloat2("Center : ", center_, 0.1f);
	//	ImGui::End();
	//}

}

void StageSelectScene::draw()const {
	GSvector2 Background_position{ 0.0f,0.0f };
	const GSvector2 ui_position{ 480.0f,-100.0f };
	const GSvector2 stage_select_stage1_position{ 600.0f,425.0f };
	const GSvector2 stage_select_boss_position{ 1133.0f,425.0f };
	gsDrawSprite2D(Texture_StageSelectSceneBackground, &Background_position, NULL, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_StageSelect, &ui_position, NULL, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_StageSelectStage1, &stage_select_stage1_position, NULL, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_StageSelectBoss, &stage_select_boss_position, NULL, NULL, NULL, NULL, 0.0f);
	gsDrawSprite2D(Texture_ArrowIcon, &arrow_icon_position_, NULL, NULL, NULL, NULL, 0.0f);


}

bool StageSelectScene::is_end()const {
	return is_end_;
}

std::string StageSelectScene::next()const {
	return next_scene_;
}

void StageSelectScene::end() {
	StageManager::get_instance().select_stage(stage_count_);
}