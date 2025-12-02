#ifndef STAGE_SELECT_SCENE_H_
#define STAGE_SELECT_SCENE_H_

#include "IScene.h"
#include "../World/World.h"

class StageSelectScene :public IScene {
public:
    //開始
    virtual void start() override;
    //更新
    virtual void update(float delta_time) override;
    //描画
    virtual void draw() const override;
    //終了しているか？
    virtual bool is_end() const override;
    //次のシーンを返す
    virtual std::string next() const override;
    //終了
    virtual void end() override;
private:
    World world_;
    bool is_end_{ false };
    bool is_start_{ false };
    float start_timer_{ 0.0f };
    std::string next_scene_;
    int max_stage_count_{};
    int stage_count_{};
    GSvector2 position_{};
    GSvector2 center_{};
};





#endif // !STAGE_SELECT_SCENE_H
