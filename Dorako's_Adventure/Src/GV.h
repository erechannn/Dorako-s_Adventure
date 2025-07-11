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

// グローバル変数を管理するシングルトン構造体
// GlobalVariableの略
struct GV {
    // インスタンスを取得
    static GV& instance() {
        static GV self;
        return self;
    }

    /* グローバル関数*/
};

#endif // !GV_H_
