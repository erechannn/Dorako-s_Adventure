#include "StageManager.h"
#include "../Assets.h"


//コンストラクタ
StageManager::StageManager() :
	selected_stage_id_(1){
	initialize_stages();
}

//ステージの初期化
void StageManager::initialize_stages() {
	stages_ = {
		{1,"草原ステージ",Octree_Stage1,Octree_Stage1Collider,{0.0f,-30.0f,0.0f},30.0f,5,StageType::NORMAL},
		{2,"草原ボスステージ",Octree_BossStage,Octree_BossStageCollider,{0.0f,-20.0f,0.0f},20.0f,1,StageType::BOSS},
		{3,"テストステージ",Octree_TestStage,Octree_TestStageCollider,{0.0f,-20.0f,0.0f},20.0f,5,StageType::NORMAL},
		{4,"タイトルステージ",Octree_TestStage,Octree_TestStageCollider,{0.0f,-20.0f,0.0f},20.0f,5,StageType::NORMAL}
	};
}

//シングルトンインスタンス
StageManager& StageManager::get_instance() {
	static StageManager instance;
	return instance;
}

//ステージの選択
void StageManager::select_stage(int stage_id) {
	if (stage_id >= 1 && stage_id <= static_cast<int>(stages_.size())) {
		selected_stage_id_ = stage_id;
	}
}
//ステージIDの取得
int StageManager::get_current_stage_id()const {
	return stages_[selected_stage_id_ - 1].id;
}
//ステージ名の取得
std::string StageManager::get_current_stage_name()const {
	return stages_[selected_stage_id_ - 1].stage_name;
}
//ステージのオクツリーの取得
UINT StageManager::get_current_stage_octree()const {
	return stages_[selected_stage_id_ - 1].stage_octree;
}
//ステージのコライダーの取得
UINT StageManager::get_current_stage_collider()const {
	return stages_[selected_stage_id_ - 1].stage_collider;
}
//ステージの星の位置の取得
GSvector3 StageManager::get_current_stage_planet_position()const {
	return stages_[selected_stage_id_ - 1].planet_position;
}
//ステージの星の半径の取得
float StageManager::get_current_stage_planet_radius()const {
	return stages_[selected_stage_id_ - 1].planet_radius;
}
int StageManager::get_clear_score()const {
	return stages_[selected_stage_id_ - 1].clear_score;
}
//ステージタイプの取得
StageManager::StageType StageManager::get_current_stage_type()const {
	return stages_[selected_stage_id_ - 1].stage_type;
}
//現在のステージがボスか
bool StageManager::is_current_stage_boss()const {
	return stages_[selected_stage_id_ - 1].stage_type == StageType::BOSS;
}
//現在のステージが通常か
bool StageManager::is_current_stage_normal()const {
	return stages_[selected_stage_id_ - 1].stage_type == StageType::NORMAL;
}
//現在のステージのデータの取得
const StageManager::StageData& StageManager::get_current_stage_data()const {
	return stages_[selected_stage_id_ - 1];
}
//総ステージ数の取得
int StageManager::get_total_stage_count()const {
	return static_cast<int>(stages_.size());
}
//タイプごとのステージ数の取得
int StageManager::get_stage_count_by_type(StageType type)const {
	int count = 0;
	for (const auto& stage : stages_) {
		if (stage.stage_type == type) {
			count++;
		}
	}
	return count;
}