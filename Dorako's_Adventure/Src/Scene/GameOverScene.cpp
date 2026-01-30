#include"GameOverScene.h"
#include "../Assets.h"
#include <iostream>

void GameOverScene::start() {
	gsLoadTexture(Texture_GameOver, "Assets/Texture/GameOverUI/GameOver.png");

	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "TitleScene";
}

void GameOverScene::update(float delta_time) {
	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) is_start_ = true;
	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了

}
void GameOverScene::draw()const {
	const GSvector2 ui_position{ 450.0f,0.0f };
	gsDrawSprite2D(Texture_GameOver, &ui_position, NULL, NULL, NULL, NULL, 0.0f);
}
bool GameOverScene::is_end()const {
	return is_end_;
}
std::string GameOverScene::next()const{
	return next_scene_;
}
void GameOverScene::end() {

}
