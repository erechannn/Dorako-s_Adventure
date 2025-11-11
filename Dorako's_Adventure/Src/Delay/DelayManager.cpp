#include "DelayManager.h"
#include <algorithm>

// シングルトンインスタンスを取得
DelayManager& DelayManager::get_instance() {
    static DelayManager instance;
    return instance;
}

// 指定秒数後にコールバックを実行
void DelayManager::add_delay(float seconds, std::function<void()> callback) {
    std::lock_guard<std::mutex> lock(tasksMutex_);

    DelayedTask task;
    task.execute_time_ = std::chrono::steady_clock::now() +
        std::chrono::milliseconds(static_cast<int>(seconds * 1000.0f));
    task.callback_ = callback;

    tasks_.push_back(task);
}

// 毎フレーム呼び出す更新関数
void DelayManager::update() {
    std::lock_guard<std::mutex> lock(tasksMutex_);

    auto now = std::chrono::steady_clock::now();

    // 実行時刻が来たタスクを処理
    for (auto it = tasks_.begin(); it != tasks_.end();) {
        if (now >= it->execute_time_) {
            // コールバックを実行
            it->callback_();
            // リストから削除
            it = tasks_.erase(it);
        }
        else {
            ++it;
        }
    }
}

// すべての遅延タスクをクリア
void DelayManager::clear() {
    std::lock_guard<std::mutex> lock(tasksMutex_);
    tasks_.clear();
}