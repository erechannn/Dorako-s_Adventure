#ifndef UI_ACTOR_H_
#define UI_ACTOR_H_

#include "../../Actor/Actor.h"

class UIActor :public Actor {
public:
    // �L���ɂ���
    void enable();
    // �����ɂ���
    void disable();
    // �L���ɂȂ��Ă��邩
    bool get_enabled() const;
    // �L��/������ݒ肷��
    bool set_enabled(bool enabled);

    // �L���ɂȂ����Ƃ��̏���
    virtual void on_enable() {};
    // �����ɂȂ����Ƃ��̏���
    virtual void on_disable() {};

protected:
    // �L���ɂȂ��Ă��邩
    bool enabled_{};
};



#endif // !UI_ACTOR_H_
