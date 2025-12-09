#include "GamePlayUI.h"
#include "../../../World/IWorld.h"
#include "../../../Assets.h"
#include<imgui/imgui.h>

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
    Actor* player = world_->find_actor("Player");
    if (player == nullptr)return;
    player_ = (Player*)player;
    fire_count_ = player_->fire_count();
    player_health_ = player_->now_health();
    player_fly_timer_ = player_->get_fly_timer();

    //if (ImGui::Begin("UI_position")) {
    //    ImGui::DragFloat2("Position : ", position_, 0.1f);
    //    ImGui::DragFloat2("Center : ", center_, 0.1f);
    //}
    //ImGui::End();

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
// —LŒø‚É‚È‚Á‚½‚Æ‚«‚Ìˆ—
void GamePlayUI::on_enable() {

}
// –³Œø‚É‚È‚Á‚½‚Æ‚«‚Ìˆ—
void GamePlayUI::on_disable() {

}

void GamePlayUI::draw_player_ui()const {
    const float max_fly_timer{ 180.0f };
    const GSvector2 fly_gauge_position{ 1580.0f,140.0f };
    gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_positions_[0], NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_positions_[1], NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_positions_[2], NULL, NULL, NULL, NULL, 0.0f);
    for (int i = 0; i < fire_count_; i++) {
        gsDrawSprite2D(Texture_FireCount, &fire_count_positions_[i], NULL, NULL, NULL, NULL, 0.0f);
    }
    gsDrawSprite2D(Texture_EmptyHealth, &heart_positions_[0], NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyHealth, &heart_positions_[1], NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyHealth, &heart_positions_[2], NULL, NULL, NULL, NULL, 0.0f);
    for (int i = 0; i < player_health_; i++) {
        gsDrawSprite2D(Texture_HealthIcon, &heart_positions_[i], NULL, NULL, NULL, NULL, 0.0f);
    }
    gsDrawSprite2D(Texture_FlyGaugeEmpty, &fly_gauge_position, NULL, NULL, NULL, NULL, 0.0f);
    float fly_gauge_rect_up = (600 / max_fly_timer) * player_fly_timer_;
    GSrect fly_gauge_rect{ 0.0f,0.0f,400.0f,fly_gauge_rect_up };
    gsDrawSprite2D(Texture_FlyGauge, &fly_gauge_position, &fly_gauge_rect, NULL, NULL, NULL, 0.0f);
}
