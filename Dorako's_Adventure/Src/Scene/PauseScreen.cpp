#include "PauseScreen.h"
#include "../Assets.h"

#include <imgui/imgui.h>

//コンストラクタ
PauseScene::PauseScene() {
	initialize();
}
//初期化
void PauseScene::initialize() {
	select_state_ = SelectState::SelectNull;
	is_end_ = false;
	is_game_play_end_ = false;
	is_confirm_ = false;
	select_num_ = 1;
	next_scene_name_ = "NULL";
}
void PauseScene::update(float delta_time) {
	//状態によって更新処理の変更
	switch (select_state_) {
	case SelectState::SelectNull:pause_update(delta_time); break;
	case SelectState::RestartGame:restart_game_update(delta_time); break;
	case SelectState::Option:option_game_update(delta_time); break;
	case SelectState::ResetStage:reset_stage_update(delta_time); break;
	case SelectState::StageSelect:stage_select_update(delta_time); break;
	case SelectState::Title:title_update(delta_time); break;
	}
	if (is_end_||is_game_play_end_) return;

	ImGui::Begin("pause");
	ImGui::Text("select_num%d", select_num_);
	ImGui::End();
}
void PauseScene::draw()const {
	//背景の描画
	draw_back_ground();
	//ポーズメニューの描画
	draw_pause_menu();
	if (select_state_ == SelectState::Option) {
		//オプション画面の描画
		draw_option();
	}
	//ゲームプレイシーンが終了する選択がされたら確認画面の描画
	if (select_state_ == SelectState::ResetStage || select_state_ == SelectState::StageSelect || select_state_ == SelectState::Title) {
		draw_confirm_window();
	}
}
//ポーズ画面を終了するか
bool PauseScene::is_end() {
	return is_end_;
}
//ゲームを終了するか
bool PauseScene::is_game_play_end() {
	return is_game_play_end_;
}
std::string PauseScene::get_next_scene_name() {
	return next_scene_name_;
}
//選択された状態が存在するか
SelectState PauseScene::to_state(int value) {
	//存在しない値なら問題のない状態に
	if (value < 1 || value >= static_cast<int>(SelectState::Max))
	{
		return SelectState::SelectNull; // デフォルト
	}
	//存在するのなら数値の状態に遷移
	else
	{
		return static_cast<SelectState>(value);
	}
}
void PauseScene::pause_update(float delta_time) {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_DOWN)) {
		select_num_ += 1;
		if (select_num_ >= static_cast<int>(SelectState::Max)) {
			select_num_ -= 1;
		}
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_UP)) {
		select_num_ -= 1;
		if (select_num_ < 1) {
			select_num_ += 1;
		}
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
		select_state_=to_state(select_num_);
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_START)) {
		is_end_ = true;
	}
}
void PauseScene::restart_game_update(float delta_time) {
	is_end_ = true;
}
void PauseScene::option_game_update(float delta_time) {

}
void PauseScene::reset_stage_update(float delta_time) {
	confirm_window_update(delta_time);
	next_scene_name_ = "GamePlayScene";
}
void PauseScene::stage_select_update(float delta_time) {
	confirm_window_update(delta_time);
	next_scene_name_ = "StageSelectScene";
}
void PauseScene::title_update(float delta_time) {
	confirm_window_update(delta_time);
	next_scene_name_ = "TitleScene";
}
void PauseScene::confirm_window_update(float delta_time) {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_RIGHT)) {
		is_confirm_ = true;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_LEFT)) {
		is_confirm_ = false;
	}
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
		if (is_confirm_) {
			is_game_play_end_ = true;
		}
		else {
			select_state_ = SelectState::SelectNull;
		}
	}
	
}

void PauseScene::draw_back_ground()const {
	GSvector2 position{ 0.0f,0.0f };
	GScolor back_ground_color{ 1.0f,1.0f,1.0f,0.5f };
	gsDrawSprite2D(Texture_BackGround, &position, NULL, NULL, &back_ground_color, NULL, 0.0f);

}
void PauseScene::draw_pause_menu()const {

}
void PauseScene::draw_option()const {

}
void PauseScene::draw_confirm_window()const {
	GSvector2 position{ 0.0f,0.0f };
	gsDrawSprite2D(Texture_BackGround, &position, NULL, NULL, NULL, NULL, 0.0f);

}