#include<GSgame.h>
#include"GV.h"
#include"Scene/SceneManager.h"

class MyGame : public::gslib::Game {
public:
    // コンストラクタ
    MyGame(int width, int height, bool full_screen) : gslib::Game{ width, height, full_screen }{
        GLOBAL.screen_size_ = GSvector2{ (GSfloat)width, (GSfloat)height };
    }
    void start()override {

    }
    void update(float delta_time)override {
        scene_manager_.update(delta_time);
    }
    void debug(float delta_time) {
        scene_manager_.debug(delta_time);
    }
    void draw() override {
        scene_manager_.draw();
    }
    void end()override {
        scene_manager_.end();
    }
private:
    SceneManager scene_manager_;
};

int main() {
    bool full_screen = !GLOBAL.is_debug_build();
    return MyGame(1920, 1080, full_screen).run();
}