#include "GamePlayUI.h"
#include "../../../World/IWorld.h"
#include "../../../Stage/StageManager.h"
#include "../../Number/NumberTexture.h"
#include "../../../Assets.h"

GamePlayUI::GamePlayUI(IWorld* world, bool enabled) {
    world_ = world;
    tag_ = "UITag";
    name_ = "GamePlayUI";
    fire_count_position_={ 1350.0f,770.0f };
    heart_position_ = { 10.0f,30.0f };
    enabled ? enable() : disable();
}
void GamePlayUI::update(float delta_time) {
    if (!enabled_)return;
    //プレイヤーを取得
    Actor* player = world_->find_actor("Player");
    if (player == nullptr)return;
    player_ = (Player*)player;
    //火の玉の回数を入手
    fire_count_ = player_->fire_count();
    //現在の体力を入手
    player_health_ = player_->now_health();
    //飛べる時間を入手
    player_fly_timer_ = player_->get_fly_timer();
    //現在の入手しているコインを入手
    now_coin_count_ = world_->get_score();
    //ステージごとのクリアに必要なコインの枚数を入手
    clear_coin_count_ = StageManager::get_instance().get_clear_score();

    //火の玉の個数分の場所を求める
    for (int i = 0; i < fire_count_; i++) {
        fire_count_positions_[i] = fire_count_position_;
        fire_count_position_.x += 150.0f;
    }
    fire_count_position_ = { 1350.0f,770.0f };

    for (int i = 0; i < player_health_; i++) {
        heart_positions_[i] = heart_position_;
        heart_position_.x += 150;
    }
    heart_position_ = { 10.0f,30.0f };

}
void GamePlayUI::draw_gui()const {
    if (!enabled_)return;
    draw_player_ui();
}
// 有効になったときの処理
void GamePlayUI::on_enable() {

}
// 無効になったときの処理
void GamePlayUI::on_disable() {

}

void GamePlayUI::draw_player_ui()const {
    const float max_fly_timer{ 180.0f };
    const GSvector2 fly_gauge_position{ 1580.0f,140.0f };
    //現在火の玉の回数の標示を消しています
    //gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_positions_[0], NULL, NULL, NULL, NULL, 0.0f);
    //gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_positions_[1], NULL, NULL, NULL, NULL, 0.0f);
    //gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_positions_[2], NULL, NULL, NULL, NULL, 0.0f);
    //for (int i = 0; i < fire_count_; i++) {
    //    gsDrawSprite2D(Texture_FireCount, &fire_count_positions_[i], NULL, NULL, NULL, NULL, 0.0f);
    //}

    // 空の体力を標示
    gsDrawSprite2D(Texture_EmptyHealth, &heart_positions_[0], NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyHealth, &heart_positions_[1], NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyHealth, &heart_positions_[2], NULL, NULL, NULL, NULL, 0.0f);
    //残り体力の標示
    for (int i = 0; i < player_health_; i++) {
        gsDrawSprite2D(Texture_HealthIcon, &heart_positions_[i], NULL, NULL, NULL, NULL, 0.0f);
    }
    //空のゲージを標示
    gsDrawSprite2D(Texture_FlyGaugeEmpty, &fly_gauge_position, NULL, NULL, NULL, NULL, 0.0f);
    //ゲージの長さを計算
    float fly_gauge_rect_up = (600 / max_fly_timer) * player_fly_timer_;
    //ゲージを標示
    GSrect fly_gauge_rect{ 0.0f,0.0f,400.0f,fly_gauge_rect_up };
    gsDrawSprite2D(Texture_FlyGauge, &fly_gauge_position, &fly_gauge_rect, NULL, NULL, NULL, 0.0f);
    
    //ステージタイプがノーマルの場合コインの標示
    if (StageManager::get_instance().get_current_stage_type() == StageManager::StageType::NORMAL) {
        const GSvector2 slash_position{ 1730.0f,30.0f };
        const GSvector2 coin_position{ 1580.0f,30.0f };
        const GSvector2 now_coin_count_position{ 1680.0f,10.0f };
        const GSvector2 clear_coin_count_position{ 1800.0f,10.0f };
        gsDrawSprite2D(Texture_Slash, &slash_position, NULL, NULL, NULL, NULL, 0.0f);
        gsDrawSprite2D(Texture_Coin, &coin_position, NULL, NULL, NULL, NULL, 0.0f);
        static const NumberTexture number{ Texture_Number,64,128 };
        number.draw(now_coin_count_position, now_coin_count_, 1);
        number.draw(clear_coin_count_position, clear_coin_count_, 1);
    }
}
