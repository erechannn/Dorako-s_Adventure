#include<GSgame.h>
#include"GV.h"
#include"Scene/SceneManager.h"
#include"World/World.h"
#include "Scene/TitleScene.h"
#include "Scene/StageSelectScene.h"
#include"Scene/GamePlayScene.h"
#include "Scene/GameOverScene.h"
#include "Scene/GameClearScene.h"
#include "Tween/Tween.h"
#include "Delay/DelayManager.h"
#include <GSeffect.h>

class MyGame : public::gslib::Game {
public:
    // コンストラクタ
    MyGame(int width, int height, bool full_screen) : gslib::Game{ width, height, full_screen }{
        GLOBAL.screen_size_ = GSvector2{ (GSfloat)width, (GSfloat)height };
    }
    void start()override {
        gsInitEffect();
        World world;
        scene_manager_.add("TitleScene", new TitleScene());
        scene_manager_.add("StageSelectScene", new StageSelectScene());
        scene_manager_.add("GamePlayScene", new GamePlayScene());
        scene_manager_.add("GameClearScene", new GameClearScene());
        scene_manager_.add("GameOverScene", new GameOverScene());
        scene_manager_.change("TitleScene");

        // マウスカーソルを非表示にする 
        //gsHideMouseCursor();

        //BGMとSEの音量を下げておく
        gsSetVolumeBGM(0.5f);
        gsSetMasterVolumeSE(0.5f);
    }
    void update(float delta_time)override {
        scene_manager_.update(delta_time);
        Tween::update(delta_time);
    }
    void debug(float delta_time) {
        scene_manager_.debug(delta_time);
    }
    void draw() override {
        scene_manager_.draw();
    }
    void end()override {
        gsFinishEffect();
        scene_manager_.end();
        scene_manager_.clear();
    }
private:
    SceneManager scene_manager_;
};

int main() {
    bool full_screen = !GLOBAL.is_debug_build();
    return MyGame(1920, 1080, full_screen).run();
}