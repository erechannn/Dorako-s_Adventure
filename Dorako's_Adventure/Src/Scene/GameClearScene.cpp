#include "GameClearScene.h"
#include "../Assets.h"
#include <iostream>

void GameClearScene::start() {
	gsLoadTexture(Texture_StageClear, "Assets/Texture/GameClearUI/StageClear.png");

	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "TitleScene";
}

void GameClearScene::update(float delta_time) {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) is_start_ = true;
	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了
}

void GameClearScene::draw()const {
	GSvector2 ui_position{ 500.0f,0.0f };
	gsDrawSprite2D(Texture_StageClear, &ui_position, NULL, NULL, NULL, NULL, 0.0f);
}
bool GameClearScene::is_end() const{
	return is_end_;
}
std::string GameClearScene::next() const{
	return next_scene_;
}
void GameClearScene::end() {

}
