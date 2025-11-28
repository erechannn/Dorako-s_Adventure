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
#include "../Actor/Enemy/BossEnemy/MiniDragon.h"
#include "../Actor/WoodBox/WoodBox.h"
#include "../UI/UIActor/UIActors/GamePlayUI.h"
#include "../Stage/StageManager.h"
#include <GSstandard_shader.h>
#include <GSeffect.h>

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
    gsLoadSkinMesh(Mesh_MiniDragon, "Assets/Mesh/Enemy/MiniDragon/MiniDragon.mshb");

    gsLoadEffect(Effeck_FireBoll, "Assets/Effect/FireBall_Orange.efkefc");

    gsLoadTexture(Texture_Skybox, "Assets/Skybox/TestStageSkybox.dds");

    gsLoadOctree(Octree_TestStage, "Assets/Stage/testStage2.oct");
    gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/testStage2Collider.oct");

    gsLoadTexture(Texture_FireCount, "Assets/Texture/GamePlayUI/fire_count_icon.png");
    gsLoadTexture(Texture_EmptyFireCount, "Assets/Texture/GamePlayUI/fire_count_empty.png");
    gsLoadTexture(Texture_HealthIcon, "Assets/Texture/GamePlayUI/health_icon.png");
    gsLoadTexture(Texture_EmptyHealth, "Assets/Texture/GamePlayUI/health_icon_empty.png");

    world_.add_actor(new Player{ &world_,{3.0f,0.0f,3.0f} });
    world_.add_actor(new DummyPlayer{ &world_ });
    //world_.add_actor(new Kuribo{ &world_,{0.0f,0.0f,0.0f} });
    world_.add_actor(new MiniDragon{ &world_,{0.0f,0.0f,0.0f} });

    UINT stage_octree = StageManager::get_instance().get_current_stage_octree();
    UINT stage_collider = StageManager::get_instance().get_current_stage_collider();

    world_.add_field(new Field{ stage_octree,stage_collider,Texture_Skybox });

    world_.add_camera(new CameraRotateAround{
                 &world_, GSvector3{ -180.0f, 1.0f, 5.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });

    world_.add_light(new Light{ &world_ });


    world_.add_actor(new GamePlayUI{ &world_,true });

    game_over_scene_.start();

	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "GameOverScene";
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

}
void GamePlayScene::update(float delta_time) {
    switch (state_) {
    case State::Playing:game_play_update(delta_time); break;
    case State::GameOver:game_over_update(delta_time); break;
    case State::Pose:pose_update(delta_time); break;
    }
    if (is_start_)          start_timer_ += delta_time; //タイマー増加
    if (start_timer_ >= 180.0f)          is_end_ = true; //シーンを終了

}
void GamePlayScene::game_play_update(float delta_time) {
    // ワールドクラスの更新
    world_.update(delta_time);
    Actor* player_actor = world_.find_actor("Player");
    Player* player = (Player*)player_actor;
    if (player == nullptr) return;
    if (player->now_health() <= 0.0f) {
        is_start_ = true;
    }
}
void GamePlayScene::game_over_update(float delta_time) {
    game_over_scene_.update(delta_time);
}
void GamePlayScene::pose_update(float delta_time) {

}
bool GamePlayScene::is_end()const {
    return is_end_;
}
std::string GamePlayScene::next()const {
    return next_scene_;
}
void GamePlayScene::draw()const {
    // ワールドの描画
    world_.draw();
}
void GamePlayScene::end() {
    gsDeleteSkinMesh(Mesh_Player);
    world_.clear();
}