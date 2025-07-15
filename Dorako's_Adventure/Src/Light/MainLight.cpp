#include "Light/MainLight.h"

// �R���X�g���N�^
MainLight::MainLight(IWorld* world) {
}

// �X�V
void MainLight::update(float delta_time) {

}

// �`��
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

    // �V���h�E�}�b�v�p�̃��C�g�̈ʒu��ݒ�
    GSvector3 shadow_map_light_position{ position[0], position[1], position[2] };
    gsSetShadowMapLightPosition(&shadow_map_light_position);
}
