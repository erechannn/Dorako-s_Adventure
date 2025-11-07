#ifndef ENEMY_EYE_H_
#define ENEMY_EYE_H_

#include <cmath>
#include <gslib.h>
#include <GStransform.h>
#include "Actor/Actor.h"

class EnemyEye : public Actor {
public:
    // コンストラクタ
    EnemyEye(IWorld* world, const GSvector3& origin, const GSvector3& forward, float angle, float range);

    // 扇形内にターゲットがいるか判定する関数
    bool isTargetWithin(const GSvector3& target) const;

    void draw() const;
    // 扇形のプロパティを設定する関数
    void setOrigin(const GSvector3& newOrigin);
    void setForward(const GSvector3& newForward);
    void setAngle(float newAngle);
    void setRange(float newRange);

private:
    GSvector3 origin;      // 扇形の中心位置
    GSvector3 forward;     // 扇形の向き
    float angle;         // 扇形の角度（度数法）
    float range;         // 扇形の範囲半径
};





#endif // !ENEMY_EYE_H_
