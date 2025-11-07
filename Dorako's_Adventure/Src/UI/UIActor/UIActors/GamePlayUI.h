#ifndef GAME_PLAY_UI_H_
#define GANE_PLAY_UI_H_

#include "../UIActor.h"
#include "../../../Actor/Player/Player.h"
#include <vector>


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

    int player_health_{};

    GSvector2 position_{};

    GSvector2 size_{};

    GSvector2 center_{};

    GSrect rect_{};

    GScolor color_{};

    GSvector2 fire_count_position_{};

    GSvector2 heart_position_{};

    GSvector2 fire_count_positions_[3];

    GSvector2 heart_positions_[3];
};





#endif // !GAME_PLAY_UI_H_
