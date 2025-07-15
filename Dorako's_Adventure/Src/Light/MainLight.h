#ifndef MAI_LIGHT_H_
#define MAI_LIGHT_H_

#include "Actor/Actor.h"

// ���C�g�N���X
class MainLight {
public:
    // �R���X�g���N�^
    MainLight(IWorld* world);
    // �X�V
    void update(float delta_time);
    // �`��
    void draw() const;
};

#endif // !LIGHT_H_
