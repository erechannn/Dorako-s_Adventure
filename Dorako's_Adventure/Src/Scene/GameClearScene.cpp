#include "GameClearScene.h"
#include "../Assets.h"
#include <iostream>

void GameClearScene::start() {
	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "TitleScene";

	std::cout << "ゲームクリア" << std::endl;
}

void GameClearScene::update(float delta_time) {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) is_start_ = true;
	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了
}

void GameClearScene::draw()const {

}
bool GameClearScene::is_end() const{
	return is_end_;
}
std::string GameClearScene::next() const{
	return next_scene_;
}
void GameClearScene::end() {

}
