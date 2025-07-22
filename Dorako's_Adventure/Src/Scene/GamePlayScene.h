#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include"IScene.h"
#include"../World/World.h"

class GamePlayScene :public IScene {
public:
    //�J�n
    virtual void start() override;
    //�X�V
    virtual void update(float delta_time) override;
    //�`��
    virtual void draw() const override;
    //�I�����Ă��邩�H
    virtual bool is_end() const override;
    //���̃V�[����Ԃ�
    virtual std::string next() const override;
    //�I��
    virtual void end() override;
private:
    World world_;
    bool is_end_{ false };
    bool is_start_{ false };
    float start_timer_{ 0.0f };
    std::string next_scene_;

};



#endif // !GAME_PLAY_SCENE_H_
