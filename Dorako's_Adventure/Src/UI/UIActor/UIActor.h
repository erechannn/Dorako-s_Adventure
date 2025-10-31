#ifndef UI_ACTOR_H_
#define UI_ACTOR_H_

#include "../../Actor/Actor.h"

class UIActor :public Actor {
public:
    // 有効にする
    void enable();
    // 無効にする
    void disable();
    // 有効になっているか
    bool get_enabled() const;
    // 有効/無効を設定する
    bool set_enabled(bool enabled);

    // 有効になったときの処理
    virtual void on_enable() {};
    // 無効になったときの処理
    virtual void on_disable() {};

protected:
    // 有効になっているか
    bool enabled_{};
};



#endif // !UI_ACTOR_H_
