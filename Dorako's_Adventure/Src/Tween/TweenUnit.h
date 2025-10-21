#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <functional>
#include <gslib.h>

// Tweenの動作1つを管理するクラスの基底クラス
class TweenUnit {
public:
    // コンストラクタ
    TweenUnit(float duration);
    // 仮想デストラクタ
    virtual ~TweenUnit() = default;
    // 更新処理
    void update(float delta_time);
    // 終了したかどうか
    bool is_finished() const;
    // コピー禁止
    TweenUnit(const TweenUnit& other) = delete;
    TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
    // 派生クラスごとの更新処理
    virtual void on_update(float progress) {}

private:
    float duration_{ 0.0f };        // アニメーション時間（フレーム）
    float elapsed_{ 0.0f };         // 経過時間（フレーム）
    bool is_finished_{ false };     // 終了したか
};

#endif

