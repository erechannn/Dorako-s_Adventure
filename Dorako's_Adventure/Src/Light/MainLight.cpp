#include "Light/MainLight.h"

// コンストラクタ
MainLight::MainLight(IWorld* world) {
}

// 更新
void MainLight::update(float delta_time) {

}

// 描画
void MainLight::draw() const {
    float ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse[4] = { 0.9f, 0.9f, 1.0f, 1.0f };
    float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float position[4] = { -100.0f, 100.0f, 100.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    // シャドウマップ用のライトの位置を設定
    GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
    gsSetShadowMapLightPosition(&shadow_map_light_position);
}
