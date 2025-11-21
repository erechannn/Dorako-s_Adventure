#include "StageSelectScene.h"
#include "../Stage/StageManager.h"


void StageSelectScene::start() {
	max_stage_count_ = StageManager::get_instance().get_total_stage_count();
	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "GamePlayScene";
}

void StageSelectScene::update(float delta_time) {
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
	StageManager::get_instance().select_stage(stage_count_);
}