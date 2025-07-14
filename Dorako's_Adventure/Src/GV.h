#ifndef GV_H_
#define GV_H_

// デバッグビルドかをチェック
#ifdef _DEBUG
namespace GVDetail {
    constexpr bool is_debug = true;
}
#else
namespace GVDetail {
    constexpr bool is_debug = false;
}
#endif


// グローバル変数にアクセス
#define GLOBAL GV::instance()

#include<gslib.h>

// グローバル変数を管理するシングルトン構造体
// GlobalVariableの略
struct GV {
    // インスタンスを取得
    static GV& instance() {
        static GV self;
        return self;
    }

    /* グローバル関数*/

    /// <summary>
    /// デバッグビルドであるかを取得する
    /// </summary>
    /// <returns>bool デバッグビルドのときtrue</returns>
    static constexpr bool is_debug_build() { return GVDetail::is_debug; }

    /* グローバル変数 */
    // ゲームの終了フラグ
    bool game_exit_flag_{ false };
    // 画面サイズ
    GSvector2 screen_size_{ 640.0f, 480.0f };
    // 現在のレベル番号
    size_t current_level_index_{ 0 };

};

#endif // !GV_H_
