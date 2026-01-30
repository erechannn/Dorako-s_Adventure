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
	option_.initialize();
	next_scene_name_ = "NULL";
	flash_time_ = 0.0f;
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

	flash_time_ += delta_time;

	//ImGui::Begin("pause");
	//ImGui::Text("select_num%d", select_num_);
	//ImGui::DragFloat2("ui_position", position_, 0.1f);
	//ImGui::DragFloat2("ui_size", size_, 0.1f);
	//ImGui::DragFloat4("ui_rect", rect_, 0.1f);
	//ImGui::End();
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
	//Aボタンが押されたら状態を変更
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
		select_state_=to_state(select_num_);
	}
	//スタートボタンもしくはBボタンが押されたらポーズ画面を終了する
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_START)||gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_B)) {
		is_end_ = true;
	}
}
void PauseScene::restart_game_update(float delta_time) {
	is_end_ = true;
}
void PauseScene::option_game_update(float delta_time) {
	//オプション画面の更新
	option_.update(delta_time);
	if (option_.is_end()) {
		option_.initialize();
		select_state_ = SelectState::SelectNull;
	}
}
void PauseScene::reset_stage_update(float delta_time) {
	//確認画面の更新
	confirm_window_update(delta_time);
	next_scene_name_ = "GamePlayScene";
}
void PauseScene::stage_select_update(float delta_time) {
	//確認画面の更新
	confirm_window_update(delta_time);
	next_scene_name_ = "StageSelectScene";
}
void PauseScene::title_update(float delta_time) {
	//確認画面の更新
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
//背景の描画
void PauseScene::draw_back_ground()const {
	GSvector2 position{ 0.0f,0.0f };
	GScolor back_ground_color{ 1.0f,1.0f,1.0f,0.5f };
	gsDrawSprite2D(Texture_BackGround, &position, NULL, NULL, &back_ground_color, NULL, 0.0f);
	gsDrawSprite2D(Texture_menu, &position, NULL, NULL, NULL, NULL, 0.0f);
}
//ポーズ画面の描画
void PauseScene::draw_pause_menu()const {
	GSvector2 size{ 0.5f,0.5f };

	GSvector2 restart_position{ 630.0f,170.0f };
	GSvector2 option_position{ 740.0f,310.0f };
	GSvector2 reset_stage_position{ 470.0f,434.0f };
	GSvector2 stage_select_position{ 424.0f,590.0f };
	GSvector2 title_position{ 550.0f,720.0f };

	//レクトごとに標示を変更
	GSrect rect{rect_.x,rect_.y,rect_.z,128.0f };
	GSrect restart_rect{ 0.0f,0.0f,700.0f,128.0f };
	GSrect option_rect{ 0.0f,128.0f,700.0f,256.0f };
	GSrect reset_stage_rect{ 0.0f,256.0f,1000.0f,384.0f };
	GSrect stage_select_rect{ 0.0f,384.0f,1100.0f,512.0f };
	GSrect title_rect{ 0.0f,512.0f,1500.0f,640.0f };

	//選択されている物を点滅させる
	GScolor restart_color{ 1.0f,1.0f,1.0f,1.0f };
	if (select_num_==1 && std::fmod(flash_time_, 40.0f) <= 10.0f)restart_color={ 1.0f,1.0f,1.0f,0.4f };
	GScolor option_color{ 1.0f,1.0f,1.0f,1.0f };
	if (select_num_==2 && std::fmod(flash_time_, 40.0f) <= 10.0f)option_color ={ 1.0f,1.0f,1.0f,0.4f };
	GScolor reset_stage_color{ 1.0f,1.0f,1.0f,1.0f };
	if (select_num_==3 && std::fmod(flash_time_, 40.0f) <= 10.0f)reset_stage_color ={ 1.0f,1.0f,1.0f,0.4f };
	GScolor stage_select_color{ 1.0f,1.0f,1.0f,1.0f };
	if (select_num_==4 && std::fmod(flash_time_, 40.0f) <= 10.0f)stage_select_color ={ 1.0f,1.0f,1.0f,0.4f };
	GScolor title_color{ 1.0f,1.0f,1.0f,1.0f };
	if (select_num_==5 && std::fmod(flash_time_, 40.0f) <= 10.0f)title_color ={ 1.0f,1.0f,1.0f,0.4f };

	gsDrawSprite2D(Texture_Pause, &restart_position, &restart_rect, NULL, &restart_color, &size, 0.0f);
	gsDrawSprite2D(Texture_Pause, &option_position, &option_rect, NULL, &option_color, &size, 0.0f);
	gsDrawSprite2D(Texture_Pause, &reset_stage_position, &reset_stage_rect, NULL, &reset_stage_color, &size, 0.0f);
	gsDrawSprite2D(Texture_Pause, &stage_select_position, &stage_select_rect, NULL, &stage_select_color, &size, 0.0f);
	gsDrawSprite2D(Texture_Pause, &title_position, &title_rect, NULL, &title_color, &size, 0.0f);

	//矢印の描画
	GSvector2 arrow_position{ 0.0f,0.0f };
	if (select_num_ == 1)arrow_position = { 590.0f,150.0f };
	if (select_num_ == 2)arrow_position = { 700.0f,300.0f };
	if (select_num_ == 3)arrow_position = { 440.0f,416.0f };
	if (select_num_ == 4)arrow_position = { 398.0f,568.0f };
	if (select_num_ == 5)arrow_position = { 522.0f,708.0f };
	gsDrawSprite2D(Texture_ArrowIcon, &arrow_position, NULL, NULL, NULL, NULL, 0.0f);
}
void PauseScene::draw_option()const {
	//オプション画面の描画
	option_.draw();
}
//確認画面の描画
void PauseScene::draw_confirm_window()const {
	GSvector2 position{ 500.0f,200.0f };
	GSvector2 size{ 0.5f,0.5f };

	draw_back_ground();
	//ボックスの標示
	gsDrawSprite2D(Texture_BackGround, &position, NULL, NULL, NULL, &size, 0.0f);

	GSrect Confirm_Window_rect{ 0.0f,0.0f,1000.0f,128.0f };
	GSvector2 confirm_window_position{ 560.0f,260.0f };
	gsDrawSprite2D(Texture_Confirm_Window, &confirm_window_position, &Confirm_Window_rect, NULL, NULL, NULL, 0.0f);

	//選択されている方を点滅させる
	GScolor yes_color{ 1.0f,1.0f,1.0f,1.0f };
	if (is_confirm_ && std::fmod(flash_time_, 40.0f) <= 10.0f)yes_color = { 0.0f,0.0f,0.0f,0.5f };
	GScolor no_color{ 1.0f,1.0f,1.0f,1.0f };
	if (!is_confirm_ && std::fmod(flash_time_, 40.0f) <= 10.0f)no_color = { 0.0f,0.0f,0.0f,0.5f };

	GSrect yes_rect{ 0.0f,128.0f,300.0f,256.0f };
	GSvector2 yes_position{ 1100.0f,520.0f };
	gsDrawSprite2D(Texture_Confirm_Window, &yes_position, &yes_rect, NULL, &yes_color, NULL, 0.0f);

	GSrect no_rect{ 0.0f,256.0f,350.0f,384.0f };
	GSvector2 no_position{ 600.0f,520.0f };
	gsDrawSprite2D(Texture_Confirm_Window, &no_position, &no_rect, NULL, &no_color, NULL, 0.0f);

	//矢印の描画
	GSvector2 arrow_position{ 570.0f,500.0f };
	if (!is_confirm_)arrow_position = { 570.0f,500.0f };
	if (is_confirm_)arrow_position = { 1060.0f,500.0f };

	gsDrawSprite2D(Texture_ArrowIcon, &arrow_position, NULL, NULL, NULL, NULL, 0.0f);

}