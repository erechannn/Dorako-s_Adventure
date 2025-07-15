#pragma once
#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <vector>
#include <functional>
#include <memory>

struct animation_event
{
    animation_event(GSuint motion, GSfloat time, std::function<void()>callback)
        :motion_{ motion }, time_{ time }, callback_{ callback }
    {
    }

    GSuint motion_;
    GSfloat time_;
    std::function<void()>callback_;
};

// アニメーション付きメッシュクラス
class AnimatedMesh {
public:
    // コンストラクタ
    AnimatedMesh(GSuint id, GSuint motion = 0, bool loop = true, GSuint num_bones = 256);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // 描画
    void draw(const GSmatrix4& mat) const;
    // モーション変更
    void change_motion(GSuint motion, bool loop = true);
    // 変換行列を設定する
    void transform(const GSmatrix4& matrix);
    // モーションが終了しているか？
    bool is_end_motion() const;
    // 現在のモーションの再生時間を取得
    float motion_time() const;
    // 現在のモーション再生時間を設定
    void motion_time(float time);
    // モーションの終了時間を取得
    float motion_end_time() const;
    // ボーンのワールド変換行列を取得
    GSmatrix4 bone_matrices(int bone_no) const;

    void add_event(GSuint motion, GSfloat time, std::function<void()>callback);

private:
    // アセットID
    GSuint id_;
    // モーション番号
    GSuint motion_;
    // モーションタイマー
    GSfloat motion_timer_;
    // モーションループフラグ
    bool motion_loop_;
    // ワールド変換行列
    GSmatrix4 transform_;
    // 前回再生したモーション番号
    GSuint prev_motion_;
    // 前回再生した最終アニメーションタイマー
    GSfloat prev_motion_timer_;
    // 補間タイマー
    GSfloat lerp_timer_;

    // ボーンのローカル用変換行列
    std::vector<GSmatrix4> local_bone_matrices_;
    // ボーンの変換行列
    std::vector<GSmatrix4> bone_matrices_;

    std::vector<std::unique_ptr<animation_event>>events_;
};

#endif // !ANIMATED_MESH_H_
