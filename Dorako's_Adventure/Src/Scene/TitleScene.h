#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "IScene.h"
#include "../World/World.h"
#include <gslib.h>

class TitleScene :public IScene {
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
    int tween_timer_{ 0 };
    std::string next_scene_;
    GSvector2 logo_position_{};
    GSvector2 first_pos_{};
    GSvector2 end_pos_{};
    GSvector2 ui_position_{};
};




#endif // !TITLE_SCENE_H_
