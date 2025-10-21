#include "TweenUnit.h"
#include <algorithm>

TweenUnit::TweenUnit(float duration) : duration_{ duration } {}

void TweenUnit::update(float delta_time) {
    // 経過時間を進める
    elapsed_ += delta_time;
    // 進捗率（0～1）を計算
    float progress = elapsed_ / duration_;
    // 進捗率が1を超えないようにする
    progress = std::min(progress, 1.0f);
    // 派生クラスごとの更新処理を行う
    on_update(progress);
    // 終了したかを判定する
    is_finished_ = progress >= 1.0f;
}

bool TweenUnit::is_finished() const {
    return is_finished_;
}

