#ifndef GAME_PLAY_UI_H_
#define GANE_PLAY_UI_H_

#include "../UIActor.h"
#include "../../../Actor/Player/Player.h"


class GamePlayUI:public UIActor {
public:
    // コンストラクタ
    GamePlayUI(IWorld* world, bool enabled);
    // 更新
    void update(float delta_time) override;
    // GUI描画
    void draw_gui() const override;

    // 有効になったときの処理
    void on_enable() override;
    // 無効になったときの処理
    void on_disable() override;

private:
    void draw_player_ui()const;

    Player* player_;

    int fire_count_{};

    GSvector2 position_{};

    GSvector2 size_{};

    GSvector2 center_{};

    GSrect rect_{};

    GScolor color_{};
};




#endif // !GAME_PLAY_UI_H_
