#include "StageSelectScene.h"

void StageSelectScene::start() {
	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "TitleScene";
}

void StageSelectScene::update(float delta_time) {
	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了

}

void StageSelectScene::draw()const {

}

bool StageSelectScene::is_end()const {
	return is_end_;
}

std::string StageSelectScene::next()const {
	return next_scene_;
}

void StageSelectScene::end() {

}