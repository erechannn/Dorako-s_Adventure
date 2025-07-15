#include"GamePlayScene.h"
#include <GSstandard_shader.h>

void GamePlayScene::start() {

    // デフォルトシェーダーの初期化
    gsInitDefaultShader();
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

    // シャドウマップの作成
    static const GSuint shadow_map_size[] = { 2048, 2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // シャドウマップを適用する距離(視点からの距離）
    gsSetShadowMapDistance(60.0f);
    // カスケードシャドウマップの分割位置を調整（デフォルトは0.5）
    gsSetShadowMapCascadeLamda(0.7f);
    // シャドウの濃さを設定(0.0:濃い～1.0:薄い)
    gsSetShadowMapAttenuation(0.0f);


	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "StageSelectScene";

}