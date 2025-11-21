#ifndef STAGE_MANAGER_H_
#define STAGE_MANAGER_H_

#include <vector>
#include <string>
#include <gslib.h>

class StageManager {
public:
    // ステージタイプの列挙型
    enum class StageType {
        NORMAL,    // 通常ステージ
        BOSS,      // ボスステージ
    };
    struct StageData {
        int id;
        std::string stage_name;
        UINT stage_octree;
        UINT stage_collider;
        GSvector3 planet_position;
        float planet_radius;
        StageType stage_type;
    };
//プライベートな関数
private:
    // コンストラクタ
    StageManager();

    // ステージ情報の初期化
    void initialize_stages();

//パブリックの関数
public:
    // シングルトンインスタンス取得
    static StageManager& get_instance();

    // ステージ選択
    void select_stage(int stage_id);

    //基本ゲッター
    int get_current_stage_id()const;
    std::string get_current_stage_name()const;
    UINT get_current_stage_octree()const;
    UINT get_current_stage_collider()const;
    GSvector3 get_current_stage_planet_position()const;
    float get_current_stage_planet_radius()const;

    //ステージタイプのゲッター
    StageType get_current_stage_type()const;
    bool is_current_stage_boss()const;
    bool is_current_stage_normal()const;

    //ステージデータ全体のゲッター
    const StageData& get_current_stage_data()const;

    // 便利な機能
    int get_total_stage_count() const;
    int get_stage_count_by_type(StageType type) const;

    // コピー・ムーブを禁止
    StageManager(const StageManager&) = delete;
    StageManager& operator=(const StageManager&) = delete;
//変数
private:
    std::vector<StageData> stages_;
    int selected_stage_id_;

};




#endif // !STAGE_MANAGER_H_
