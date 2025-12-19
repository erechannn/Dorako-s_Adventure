#include "PauseScreen.h"

//コンストラクタ
PauseScene::PauseScene() {
	initialize();
}
//初期化
void PauseScene::initialize() {
	select_state_ = SelectState::SelectNull;
	is_end_ = false;
	select_num_ = 0;
}
void PauseScene::update(float delta_time) {
	//状態によって更新処理の変更
	switch (select_state_) {
	case SelectState::SelectNull:pause_update(delta_time);
	case SelectState::RestartGame:restart_game_update(delta_time);
	case SelectState::Option:option_game_update(delta_time);
	case SelectState::ResetStage:reset_stage_update(delta_time);
	case SelectState::StageSelect:stage_select_update(delta_time);
	case SelectState::Title:title_update(delta_time);
	}
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
//シーンが終了するか
bool PauseScene::is_end() {
	return is_end_;
}
//選択された状態が存在するか
SelectState PauseScene::to_state(int value) {
	//存在しない値なら問題のない状態に
	if (value < 0 || value >= static_cast<int>(SelectState::Max))
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

}
void PauseScene::restart_game_update(float delta_time) {

}
void PauseScene::option_game_update(float delta_time) {

}
void PauseScene::reset_stage_update(float delta_time) {

}
void PauseScene::stage_select_update(float delta_time) {

}
void PauseScene::title_update(float delta_time) {

}
void PauseScene::draw_back_ground()const {

}
void PauseScene::draw_pause_menu()const {

}
void PauseScene::draw_option()const {

}
void PauseScene::draw_confirm_window()const {

}