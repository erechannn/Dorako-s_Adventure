#ifndef MAI_LIGHT_H_
#define MAI_LIGHT_H_

#include "Actor/Actor.h"

// ライトクラス
class MainLight {
public:
    // コンストラクタ
    MainLight(IWorld* world);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
};

#endif // !LIGHT_H_
