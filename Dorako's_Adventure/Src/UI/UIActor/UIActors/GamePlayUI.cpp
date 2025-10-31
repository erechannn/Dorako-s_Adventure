#include "GamePlayUI.h"
#include "../../../World/IWorld.h"
#include "../../../Assets.h"
#include<imgui/imgui.h>

GamePlayUI::GamePlayUI(IWorld* world, bool enabled) {
    world_ = world;
    tag_ = "UITag";
    name_ = "GamePlayUI";
    player_ = new Player{ world,{0.0f,0.0f,0.0f} };
    enabled ? enable() : disable();
}
void GamePlayUI::update(float delta_time) {
    if (!enabled_)return;
    fire_count_ = player_->fire_count();

    if (ImGui::Begin("UI_position")) {
        ImGui::DragFloat2("Position : ", position_, 0.1f);
        ImGui::DragFloat2("Center : ", center_, 0.1f);
        ImGui::End();
    }
}
void GamePlayUI::draw_gui()const {
    if (!enabled_)return;
    draw_player_ui();
}
// óLå¯Ç…Ç»Ç¡ÇΩÇ∆Ç´ÇÃèàóù
void GamePlayUI::on_enable() {

}
// ñ≥å¯Ç…Ç»Ç¡ÇΩÇ∆Ç´ÇÃèàóù
void GamePlayUI::on_disable() {

}

void GamePlayUI::draw_player_ui()const {
    static GSvector2 fire_count_position_1{ 1650.0f,770.0f };
    static GSvector2 fire_count_position_2{ 1500.0f,770.0f };
    static GSvector2 fire_count_position_3{ 1350.0f,770.0f };
    gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_position_1, NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_position_2, NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_EmptyFireCount, &fire_count_position_3, NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_FireCount, &fire_count_position_1, NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_FireCount, &fire_count_position_2, NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_FireCount, &fire_count_position_3, NULL, NULL, NULL, NULL, 0.0f);

}
