#include"GamePlayScene.h"
#include"../Camera/CameraRotateAround.h"
#include"../Camera/MainCamera.h"
#include "../Camera/TestCamera.h"
#include "../Camera/DummyPlayer.h"
#include"../World/Field.h"
#include"../Light/MainLight.h"
#include"../Assets.h"
#include"../Actor/Player/Player.h"
#include "../Actor/Enemy/Kuribo.h"
#include "../Actor/WoodBox/WoodBox.h"
#include <GSstandard_shader.h>

void GamePlayScene::start() {

    // デフォルトシェーダーの初期化
    gsInitDefaultShader();
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

    //// シャドウマップの作成
    //static const GSuint shadow_map_size[] = { 2048, 2048 };
    //gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    //// シャドウマップを適用する距離(視点からの距離）
    //gsSetShadowMapDistance(60.0f);
    //// カスケードシャドウマップの分割位置を調整（デフォルトは0.5）
    //gsSetShadowMapCascadeLamda(0.7f);
    //// シャドウの濃さを設定(0.0:濃い～1.0:薄い)
    //gsSetShadowMapAttenuation(0.0f);

    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");

    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");

    // フォグの設定
    const static float fog_color[4]{ 0.190f, 0.231f, 0.238f, 1.0f };// Fogの色 R190  G231 B238
    const static float fog_start{ 10.0f };
    const static float fog_end{ 100.0f };
    glFogi(GL_FOG_MODE, GL_LINEAR);     // 線形フォグ
    glFogfv(GL_FOG_COLOR, fog_color);   // フォグの色
    glFogf(GL_FOG_START, fog_start);    // フォグの開始位置（視点からの距離）
    glFogf(GL_FOG_END, fog_end);        // フォグの終了位置（視点からの距離）
    glEnable(GL_FOG);                   // フォグを有効にする

    //デフォルトシェーダーの初期化（メッシュファイルを読み込む前に有効にする）
    gsInitDefaultShader();

    gsLoadSkinMesh(Mesh_Player, "Assets/Mesh/Player/DragonSpark.mshb");
    gsLoadSkinMesh(Mesh_Kuribo, "Assets/Mesh/Enemy/FirePig/FirePig.mshb");
    gsLoadMesh(Mesh_WoodBox, "Assets/Mesh/WoodBox/WoodBox.mshb");
    gsLoadMesh(Mesh_Planet, "Assets/Mesh/Planet/planet_.mshb");

    gsLoadTexture(Texture_Skybox, "Assets/Skybox/TestStageSkybox.dds");
    gsLoadOctree(Octree_TestStage, "Assets/Stage/testStage2.oct");
    gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/testStage2Collider.oct");

    world_.add_actor(new Player{ &world_,{3.0f,0.0f,3.0f} });
    world_.add_actor(new DummyPlayer{ &world_ });
    //world_.add_actor(new Kuribo{ &world_,{0.0f,0.0f,0.0f} });
    world_.add_actor(new WoodBox{ &world_,{20.0f,-20.0f,5.0f} });
    world_.add_actor(new WoodBox{ &world_,{0.0f,0.0f,0.0f} });

    world_.add_field(new Field{ Octree_TestStage,Octree_TestStageCollider,Texture_Skybox });

    world_.add_camera(new CameraRotateAround{
                 &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });

    world_.add_light(new Light{ &world_ });

	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "StageSelectScene";
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

}
void GamePlayScene::update(float delta_time) {
    // ワールドクラスの更新
    world_.update(delta_time);
    if (is_start_)          start_timer_ += delta_time; //タイマー増加
    if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了

}
bool GamePlayScene::is_end()const {
    return is_end_;
}
std::string GamePlayScene::next()const {
    return "";
}
void GamePlayScene::draw()const {
    // ワールドの描画
    world_.draw();
}
void GamePlayScene::end() {
    gsDeleteSkinMesh(Mesh_Player);
    world_.clear();
}