#ifndef DELAY_MANAGER_H
#define DELAY_MANAGER_H

#include <functional>
#include <vector>
#include <chrono>
#include <mutex>

class DelayManager {
public:
    // シングルトンインスタンスを取得
    static DelayManager& get_instance();

    // 指定秒数後にコールバックを実行
    void add_delay(float seconds, std::function<void()> callback_);

    // 毎フレーム呼び出す更新関数
    void update();

    // すべての遅延タスクをクリア
    void clear();

    // コピー・ムーブを禁止(シングルトンパターン)
    DelayManager(const DelayManager&) = delete;
    DelayManager& operator=(const DelayManager&) = delete;
    DelayManager(DelayManager&&) = delete;
    DelayManager& operator=(DelayManager&&) = delete;

private:
    // プライベートコンストラクタ(シングルトンパターン)
    DelayManager() = default;
    ~DelayManager() = default;

    // 遅延実行する情報を保持する構造体
    struct DelayedTask {
        std::chrono::steady_clock::time_point execute_time_;
        std::function<void()> callback_;
    };

    std::vector<DelayedTask> tasks_;
    std::mutex tasksMutex_;  // スレッドセーフにするためのミューテックス
};

// グローバルな便利関数
namespace Delay {
    // 簡単に呼び出せるヘルパー関数
    inline void after(float seconds, std::function<void()> callback_) {
        DelayManager::get_instance().add_delay(seconds, callback_);
    }
}

#endif // DELAY_MANAGER_H