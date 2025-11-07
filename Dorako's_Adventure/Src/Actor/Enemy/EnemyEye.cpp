#include "Actor/Enemy/EnemyEye.h"
#include <opengl_ext.h>

float M_PI = 3.141592653589793238;

// FanShape クラスのコンストラクタ
EnemyEye::EnemyEye(IWorld* world, const GSvector3& origin, const GSvector3& forward, float angle, float range)
    : origin(origin), forward(forward.normalized()), angle(angle), range(range) {
}

// 扇形内にターゲットがいるか判定する関数
bool EnemyEye::isTargetWithin(const GSvector3& target) const {
    GSvector3 toTarget = target - origin;
    float distance = toTarget.length();

    // 範囲外の場合は false を返す
    if (distance > range) {
        return false;
    }

    GSvector3 normalizedToTarget = toTarget.normalized();
    float cosTheta = forward.dot(normalizedToTarget);
    float angleRadians = angle * (M_PI / 180.0f);
    float cosAngle = std::cos(angleRadians / 2.0f);

    // 扇形の角度内であれば true を返す
    return cosTheta >= cosAngle;
}

void EnemyEye::draw() const {
    int segments = 20;  // ワイヤーフレームの分割数
    float angleRad = angle * (M_PI / 180.0f);  // 度をラジアンに変換
    float halfAngle = angleRad / 2.0f;         // 半角のラジアン

    // 扇形の中心から描画
    GSvector3 rightVec = GSvector3(-forward.z, 0.5f, forward.x).normalized();  // forward に直交する方向

    // 中心から右側の境界角
    GSvector3 startVec = forward * std::cos(halfAngle) + rightVec * std::sin(halfAngle);
    startVec.normalize();

    glBegin(GL_LINE_STRIP);  // ワイヤーフレーム用にラインストリップを開始
    glVertex3f(origin.x, 0.5f, origin.z);  // 扇形の中心

    // 境界角の半分から反時計回りに線を描く
    for (int i = 0; i <= segments; ++i) {
        float theta = -halfAngle + i * (angleRad / segments);
        GSvector3 edgeDir = forward * std::cos(theta) + rightVec * std::sin(theta);
        edgeDir.normalize();

        GSvector3 point = origin + edgeDir * range;
        glVertex3f(point.x, 0.5f, point.z);
    }

    glVertex3f(origin.x, 0.5f, origin.z);  // 最後に中心に戻る
    glEnd();  // 描画終了
}

// プロパティの設定メソッド
void EnemyEye::setOrigin(const GSvector3& newOrigin) {
    origin = newOrigin;
}

void EnemyEye::setForward(const GSvector3& newForward) {
    forward = newForward.normalized();
}

void EnemyEye::setAngle(float newAngle) {
    angle = newAngle;
}

void EnemyEye::setRange(float newRange) {
    range = newRange;
}
