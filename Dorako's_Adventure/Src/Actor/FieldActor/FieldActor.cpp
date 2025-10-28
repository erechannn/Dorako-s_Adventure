#include "FieldActor.h"
#include "../../World/IWorld.h"
#include "../../World/Field.h"
#include "../../Shape/Line.h"
#include "../../Shape/Ray.h"
#include "../../Collider/BoundingSphere.h"
#include "../../Assets.h"

// 描画
void FieldActor::draw() const {
    // メッシュの描画
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(mesh_);
    glPopMatrix();
}

// レイとの衝突判定
bool FieldActor::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane) const {
    // レイのパラメータを線分に変換
    Line line;
    line.start = ray.position;
    line.end = ray.position + (ray.direction.normalized() * max_distance);
    // 線分との衝突判定を行う
    return collide(line, intersect, plane);
}

// 線分との衝突判定
bool FieldActor::collide(const Line& line, GSvector3* intersect, GSplane* plane) const {
    // ローカル座標系の線分を求める
    Line local_line;
    local_line.start = transform_.inverseTransformPoint(line.start);
    local_line.end = transform_.inverseTransformPoint(line.end);
    // メッシュと線分が交差したか？
    if (gsCollisionMeshAndLine(gsGetMesh(mesh_collider_), &local_line.start, &local_line.end, intersect, plane) == GS_TRUE) {
        if (intersect != nullptr) {
            // 交点をワールド座標系に変換
            *intersect = transform_.transformPoint(*intersect);
        }
        if (plane != nullptr) {
            // 平面の方程式をワールド座標系に変換
            const GSmatrix4& local_to_world_matrix = transform_.localToWorldMatrix();
            gsPlaneTransform(plane, plane, &local_to_world_matrix);
        }
        return true;
    }
    return false;
}

// 球体との衝突判定
bool FieldActor::collide(const BoundingSphere& sphere, GSvector3* collided_center) const {
    // ローカル座標系の球体を求める
    BoundingSphere local_sphere;
    local_sphere.center = transform_.inverseTransformPoint(sphere.center);
    local_sphere.radius = sphere.radius;
    // メッシュと球体が衝突したか？
    GSvector3 local_collided_center;
    if (gsCollisionMeshAndSphere(gsGetMesh(mesh_collider_), &local_sphere.center, local_sphere.radius, &local_collided_center) == GS_TRUE) {
        if (collided_center != nullptr) {
            // 衝突判定後の中心座標をワールド座標系に変換
            *collided_center = transform_.transformPoint(local_collided_center);
        }
        return true;
    }
    return false;
}

void FieldActor::gravity_update(float delta_time) {
    //地上についていたら無視
    if (is_ground_)return;
    if (is_zero_gravity_)return;
    GSvector3 planet_position{ 0.0f,-20.0f,0.0f };//星の中心
    GSvector3 position = transform_.position();//自分の位置
    GSvector3 gravity = position - planet_position;//方向ベクトルを求める
    gravity = gravity.normalize();//単一ベクトル
    gravity_velocity_ += gravity * gravity_ * delta_time;
    transform_.translate(gravity_velocity_, GStransform::Space::World);//移動

}
void FieldActor::collide_ground() {
    // 地面との衝突判定（線分との交差判定）
    GSvector3 line_start = transform_.position() + transform_.up() * height_;
    GSvector3 down_direction = -transform_.up();
    GSvector3 line_end = transform_.position() + (down_direction * foot_offset_);
    GSvector3 collision_point;    // 衝突した地面との交点
    GSplane ground_plane;         // 衝突した地面の平面
    if (gsOctreeCollisionLine(gsGetOctree(Octree_TestStageCollider),
        &line_start, &line_end, &collision_point, &ground_plane)) {
        // 衝突した位置に座標を補正する
        transform_.position(collision_point);
        //重力を無効
        is_ground_ = true;
        gravity_velocity_ = GSvector3::zero();
        // 斜面に合わせてキャラクタを傾かせる
        GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
        GSvector3 up = transform_.position() - planet_position;
        GSvector3 left = GSvector3::cross(up, transform_.forward());
        GSvector3 forward = GSvector3::cross(left, up);
        transform_.rotation(GSquaternion::lookRotation(forward, up));
    }
    else  is_ground_ = false;

}
void FieldActor::collide_actor(Actor& other) {
    // ｙ座標を除く座標を求める
    GSvector3 position = transform_.position();
    GSvector3 target = other.transform().position();
    // 相手との距離
    float distance = GSvector3::distance(position, target);
    // 衝突判定球の半径同士を加えた長さを求める
    float length = collider_.radius + other.collider().radius;
    // 衝突判定球の重なっている長さを求める
    float overlap = length - distance;
    // 重なっている部分の半分の距離だけ離れる移動量を求める
    GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
    transform_.translate(v, GStransform::Space::World);
}

