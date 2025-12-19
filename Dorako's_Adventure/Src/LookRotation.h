#pragma once

#include <gslib.h>
#include <cmath>

inline GSquaternion lookRotation(const GSvector3& view, const GSvector3& up) {
    const float kEpsilon{ 1e-05f };

    // Forward (Z軸) の確定
    // View方向をZ軸とします（+Zが前方）
    GSvector3 forward = view;
    float forwardMagSq = forward.sqrMagnitude();

    // ゼロベクトル対策
    if (forwardMagSq < (kEpsilon * kEpsilon)) {
        // Viewがない場合はデフォルトの前方(0,0,1)とする
        forward = GSvector3{ 0.0f, 0.0f, 1.0f };
    } else {
        forward.normalize();
    }

    // 右手系: Y(Up) x Z(Forward) = X(Right)
    GSvector3 right = GSvector3::cross(up, forward);

    // ForwardとUpがほぼ平行で、外積が作れない場合
    if (right.sqrMagnitude() < (kEpsilon * kEpsilon)) {
        if (std::abs(forward.y) > 0.999f) {
            right = GSvector3::cross(GSvector3{ 1.0f, 0.0f, 0.0f }, forward);
        } else {
            right = GSvector3::cross(GSvector3{ 0.0f, 1.0f, 0.0f }, forward);
        }
    }

    // 正規直交基底(Orthonormal Basis)の確定
    right.normalize();

    // 右手系: Z(Forward) x X(Right) = Y(Up)
    GSvector3 realUp = GSvector3::cross(forward, right);
    // forwardとrightは正規化され直交しているため、realUpも正規化済みとなります

    // 行列成分からクォータニオンへの変換
    // 行優先(Row-Major)かつ、右手系(+Z前方)の行列:
    // Row 0: Right  (right.x,  right.y,  right.z)
    // Row 1: Up     (realUp.x, realUp.y, realUp.z)
    // Row 2: Forward(forward.x,forward.y,forward.z)

    float m00 = right.x;   float m01 = right.y;   float m02 = right.z;
    float m10 = realUp.x;  float m11 = realUp.y;  float m12 = realUp.z;
    float m20 = forward.x; float m21 = forward.y; float m22 = forward.z;

    GSquaternion result;
    // トレース（対角和）
    float trace = m00 + m11 + m22;

    if (trace > 0.0f) {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        result.w = 0.25f / s;
        result.x = (m12 - m21) * s;
        result.y = (m20 - m02) * s;
        result.z = (m01 - m10) * s;
    } else {
        if (m00 > m11 && m00 > m22) {
            float s = 2.0f * std::sqrt(1.0f + m00 - m11 - m22);
            float recip = 1.0f / s;
            result.w = (m12 - m21) * recip;
            result.x = 0.25f * s;
            result.y = (m01 + m10) * recip;
            result.z = (m02 + m20) * recip;
        } else if (m11 > m22) {
            float s = 2.0f * std::sqrt(1.0f + m11 - m00 - m22);
            float recip = 1.0f / s;
            result.w = (m20 - m02) * recip;
            result.x = (m01 + m10) * recip;
            result.y = 0.25f * s;
            result.z = (m12 + m21) * recip;
        } else {
            float s = 2.0f * std::sqrt(1.0f + m22 - m00 - m11);
            float recip = 1.0f / s;
            result.w = (m01 - m10) * recip;
            result.x = (m02 + m20) * recip;
            result.y = (m12 + m21) * recip;
            result.z = 0.25f * s;
        }
    }
    return result;
}
