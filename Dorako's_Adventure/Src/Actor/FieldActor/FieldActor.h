#ifndef FIELD_ACTOR_H_
#define FIELD_ACTOR_H_

#include "../Actor.h"

// 前方宣言
class Ray;
class Line;
class BoundingSphere;

// フィールド用のアクタークラス
class FieldActor : public Actor {
public:
    // 描画
    virtual void draw() const override;

    // レイとの衝突判定
    virtual bool collide(
        const Ray& ray, float max_distance,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // 線分との衝突判定
    virtual bool collide(
        const Line& line,
        GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;
    // 球体との衝突判定
    virtual bool collide(
        const BoundingSphere& sphere,
        GSvector3* intersect = nullptr) const;

protected:
    void gravity_update(float delta_time);
    void collide_ground();
    void collide_actor(Actor& actor);

    // メッシュ
    GSuint          mesh_{ 0 };
    // メッシュコライダ
    GSuint          mesh_collider_{ 0 };

    //重力量
    float gravity_{ -0.003 };
    //重力による移動量
    GSvector3 gravity_velocity_{ 0.0f,0.0f,0.0f };
    int max_health_{ 1 };
    int health_{ 1 };
    //足元のオフセット
    float foot_offset_{ 0.1f };
    //キャラの高さ
    float height_{ 1.0f };
    //重力を適応するか
    bool is_ground_{ false };
    bool is_zero_gravity_{ false };

};

#endif

