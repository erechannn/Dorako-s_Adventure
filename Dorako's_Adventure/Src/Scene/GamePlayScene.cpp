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
#include "../Actor/Coin/Coin.h"
#include "../UI/UIActor/UIActors/GamePlayUI.h"
#include "../Stage/StageManager.h"
#include <GSstandard_shader.h>
#include <GSeffect.h>

#include <imgui/imgui.h>

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

    gsLoadLightmap(0, "Assets/Lightmap/testLightmap.txt");

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

    // シルエット用のシェーダーを読み込む
    gsLoadShader(Shader_Silhouette, "Assets/Shader/RenderTexture.vert", "Assets/Shader/Silhouette.frag");
    // レンダーターゲットを作成(ベースシーン用）
    gsCreateRenderTarget(Rt_BaseScene, 1920, 1080, GS_TRUE, GS_FALSE, GS_FALSE);
    // レンダーターゲットを作成(背景のみ）
    gsCreateRenderTarget(Rt_Field, 1920, 1080, GS_TRUE, GS_FALSE, GS_FALSE);
    // レンダーターゲットを作成(シルエット用）
    gsCreateRenderTarget(Rt_Silhouette, 1920, 1080, GS_TRUE, GS_FALSE, GS_FALSE);
    // レンダーターゲットを作成（デプスバッファ用）
    gsCreateRenderTarget(Rt_Detpth, 1920, 1080, GS_FALSE, GS_TRUE, GS_FALSE);
    // 各レンダーターゲットにデプスバッファをアタッチ
    gsAttachDepthBufferToRenderTarget(Rt_BaseScene, Rt_Detpth);
    gsAttachDepthBufferToRenderTarget(Rt_Field, Rt_Detpth);
    gsAttachDepthBufferToRenderTarget(Rt_Silhouette, Rt_Detpth);




    //デフォルトシェーダーの初期化（メッシュファイルを読み込む前に有効にする）
    gsInitDefaultShader();

    //メッシュのロード
    gsLoadSkinMesh(Mesh_Player, "Assets/Mesh/Player/DragonSpark.mshb");
    gsLoadSkinMesh(Mesh_Kuribo, "Assets/Mesh/Enemy/FirePig/FirePig.mshb");
    gsLoadMesh(Mesh_WoodBox, "Assets/Mesh/WoodBox/WoodBox.mshb");
    gsLoadMesh(Mesh_Coin, "Assets/Mesh/Coin/Coin.mshb");
    gsLoadSkinMesh(Mesh_MiniDragon, "Assets/Mesh/Enemy/MiniDragon/MiniDragon.mshb");

    //エフェクトのロード
    gsLoadEffect(Effect_FireBoll, "Assets/Effect/FireBall_Orange.efkefc");
    //スカイボックスのロード
    gsLoadTexture(Texture_Skybox, "Assets/Skybox/CosmicSkybox.dds");
    //オクツリーのロード
    gsLoadOctree(Octree_TestStage, "Assets/Stage/TestStage/TestStageOctree.oct");
    gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/TestStage/TestStageOctreeCollider.oct");
    gsLoadOctree(Octree_Stage1, "Assets/Stage/Stage1/Stage1Octree.oct");
    gsLoadOctree(Octree_Stage1Collider, "Assets/Stage/Stage1/Stage1OctreeCollider.oct");
    gsLoadOctree(Octree_BossStage, "Assets/Stage/BossStage/BossStageOctree.oct");
    gsLoadOctree(Octree_BossStageCollider, "Assets/Stage/BossStage/BossStageOctreeCollider.oct");
    //テクスチャのロード
    gsLoadTexture(Texture_FireCount, "Assets/Texture/GamePlayUI/fire_count_icon.png");
    gsLoadTexture(Texture_EmptyFireCount, "Assets/Texture/GamePlayUI/fire_count_empty.png");
    gsLoadTexture(Texture_HealthIcon, "Assets/Texture/GamePlayUI/health_icon.png");
    gsLoadTexture(Texture_EmptyHealth, "Assets/Texture/GamePlayUI/health_icon_empty.png");
    gsLoadTexture(Texture_FlyGaugeEmpty, "Assets/Texture/GamePlayUI/FlyGaugeEmpty.png");
    gsLoadTexture(Texture_FlyGauge, "Assets/Texture/GamePlayUI/FlyGauge.png");
    gsLoadTexture(Texture_Number, "Assets/Texture/num.png");
    gsLoadTexture(Texture_BackGround, "Assets/Texture/Black.png");
    gsLoadTexture(Texture_Coin, "Assets/Texture/GamePlayUI/coin.png");
    gsLoadTexture(Texture_Slash, "Assets/Texture/GamePlayUI/Slash.png");
    gsLoadTexture(Texture_menu, "Assets/Texture/menu.png");
    gsLoadTexture(Texture_Pause, "Assets/Texture/PauseUI/PauseTexture.png");
    gsLoadTexture(Texture_Confirm_Window, "Assets/Texture/PauseUI/ConfirmWindow.png");
    gsLoadTexture(Texture_ArrowIcon, "Assets/Texture/ArrowIcon.png");
    gsLoadTexture(Texture_Option, "Assets/Texture/OptionUI/OptionTexture.png");
    gsLoadTexture(Texture_SoundGauge, "Assets/Texture/OptionUI/SoundGauge.png");
    gsLoadTexture(Texture_SoundGaugeEmpty, "Assets/Texture/OptionUI/SoundGaugeEmpty.png");
    gsLoadTexture(Texture_kaihatu, "Assets/Texture/kaihatu.png");
    //サウンドのロード
    gsLoadBGM(BGM_GamePlayBGM, "Assets/Sound/BGM/GamePlayBGM.oga", GS_TRUE);
    gsLoadSE(SE_Select, "Assets/Sound/SE/SelectSE.wav", 1, GS_TRUE);
    gsLoadSE(SE_WalkSound,"Assets/Sound/SE/WalkSound.wav",10,GS_TRUE);
    gsLoadSE(SE_Jump,"Assets/Sound/SE/JumpSE.wav",1,GS_TRUE);
    //プレイヤーを追加
    world_.add_actor(new Player{ &world_,{0.0f,0.0f,0.0f} });
    //カメラ用の追加
    world_.add_actor(new DummyPlayer{ &world_ });
    //ステージごとに敵とコインを追加
    if (StageManager::get_instance().get_current_stage_id() == 1) {
        world_.add_actor(new Kuribo{ &world_,{29.6f,-34.0f,1.4f} });
        world_.add_actor(new Kuribo{ &world_,{27.0f,-24.0f,9.0f} });
        world_.add_actor(new Kuribo{ &world_,{28.0f,-20.7f,-5.0f} });
        world_.add_actor(new Kuribo{ &world_,{10.0f,-58.0f,0.0f} });
        world_.add_actor(new Kuribo{ &world_,{0.0f,-58.6f,-9.0f} });
        world_.add_actor(new Kuribo{ &world_,{-10.0f,-58.2f,0.0f} });

        world_.add_actor(new Coin{ &world_,{-0.806f,5.643f,5.894f} });
        world_.add_actor(new Coin{ &world_,{31.0f,-25.08f,1.43f} });
        world_.add_actor(new Coin{ &world_,{-2.17f,-15.0f,-26.0f} });
        world_.add_actor(new Coin{ &world_,{0.0f,-61.0f,1.0f} });
        world_.add_actor(new Coin{ &world_,{-2.0f,-10.2f,22.5f} });
    }
    if (StageManager::get_instance().get_current_stage_id() == 2) {
        world_.add_actor(new MiniDragon{ &world_,{0.0f,0.0f,0.0f} });
    }
    if (StageManager::get_instance().get_current_stage_id() == 3) {
        world_.add_actor(new Coin{ &world_,{1.3f,-1.59f,7.37f} });
        world_.add_actor(new Coin{ &world_,{20.0f,-20.0f,0.0f} });
        world_.add_actor(new Coin{ &world_,{-20.0f,-20.0f,0.0f} });
        world_.add_actor(new Coin{ &world_,{0.0f,-20.0f,20.0f} });
        world_.add_actor(new Coin{ &world_,{0.0f,-20.0f,-20.0f} });
        world_.add_actor(new Kuribo{ &world_,{1.3f,-1.59f,7.37f} });
        world_.add_actor(new Kuribo{ &world_,{20.0f,-20.0f,0.0f} });
        world_.add_actor(new Kuribo{ &world_,{-20.0f,-20.0f,0.0f} });
        world_.add_actor(new Kuribo{ &world_,{0.0f,-20.0f,19.0f} });
        world_.add_actor(new Kuribo{ &world_,{0.0f,-20.0f,-20.0f} });
    }
    //ステージ情報を入手
    UINT stage_octree = StageManager::get_instance().get_current_stage_octree();
    UINT stage_collider = StageManager::get_instance().get_current_stage_collider();
    //ステージを追加
    world_.add_field(new Field{ stage_octree,stage_collider,Texture_Skybox });
    //カメラの追加
    world_.add_camera(new CameraRotateAround{
                 &world_, GSvector3{ 0.0f, 1.0f, 3.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    //ライトの追加
    world_.add_light(new Light{ &world_ });
    //UIの追加
    world_.add_actor(new GamePlayUI{ &world_,true });
    //ポーズ画面の初期化
    pause_scene_.initialize();
    //状態の初期化
    state_ = State::Playing;
    //終了フラグの初期化
	is_end_ = false;
	is_start_ = false;
    //終了タイマーの初期化
	start_timer_ = 0.0f;
	next_scene_ = "GameOverScene";
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);
    //BGMを鳴らす
    gsPlayBGM(BGM_GamePlayBGM);
}
void GamePlayScene::update(float delta_time) {
    //状態ごとに更新を変更する
    switch (state_) {
    case State::Playing:game_play_update(delta_time); break;
    case State::Pause:pause_update(delta_time); break;
    }
    if (is_start_)          start_timer_ += delta_time; //タイマー増加
    if (start_timer_ >= 120.0f)          is_end_ = true; //シーンを終了

    //ImGui::Begin("Coin_Position");
    //ImGui::DragFloat3("Coin_position", coin_position_, 0.01f);
    //ImGui::End();

    //Actor* coin = world_.find_actor("Coin");
    //coin->transform().position(coin_position_);

}
void GamePlayScene::game_play_update(float delta_time) {
    // ワールドクラスの更新
    world_.update(delta_time);
    //スタートボタンでポーズ画面へ
    if (gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_START)) {
        pause_scene_.initialize();
        state_ = State::Pause;
    }
    if (gsGetKeyState(GKEY_ESCAPE)) {
        pause_scene_.initialize();
        state_ = State::Pause;

    }
    //ゲームクリアの条件
    if (world_.get_score() >= StageManager::get_instance().get_clear_score() &&
        StageManager::get_instance().get_current_stage_type() == StageManager::StageType::NORMAL) {

        next_scene_ = "GameClearScene";
        is_start_ = true;
    }
    //ゲームオーバーの条件
    Actor* player_actor = world_.find_actor("Player");
    Player* player = (Player*)player_actor;
    if (player == nullptr) return;
    if (player->now_health() <= 0.0f) {
        next_scene_ = "GameOverScene";
        is_start_ = true;
    }
}
void GamePlayScene::pause_update(float delta_time) {
    //ポーズ画面の更新
    pause_scene_.update(delta_time);
    //ポーズ画面空終了フラグが立ったらゲームに戻る
    if (pause_scene_.is_end()) {
        state_ = State::Playing;
    }
    //ゲーム終了が選ばれたらゲーム終了
    if (pause_scene_.is_game_play_end()) {
        //次のシーンを得る
        next_scene_ = pause_scene_.get_next_scene_name();
        //終了フラグを立てる
        is_start_ = true;
    }
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
    if (state_ == State::Pause) {
        //ポーズ画面の描画
        pause_scene_.draw();
    }
}
void GamePlayScene::end() {
    //ワールドをクリア
    world_.clear();
    //BGMを切る
    gsStopBGM();
    //ロードした物を削除
    gsDeleteShader(Shader_Silhouette);

    gsDeleteSkinMesh(Mesh_Player);
    gsDeleteSkinMesh(Mesh_Kuribo);
    gsDeleteMesh(Mesh_Coin);
    gsDeleteMesh(Mesh_MiniDragon);

    gsDeleteOctree(Octree_BossStage);
    gsDeleteOctree(Octree_BossStageCollider);
    gsDeleteOctree(Octree_Stage1);
    gsDeleteOctree(Octree_Stage1Collider);
    gsDeleteOctree(Octree_TestStage);
    gsDeleteOctree(Octree_TestStageCollider);

    gsDeleteTexture(Texture_EmptyFireCount);
    gsDeleteTexture(Texture_EmptyHealth);
    gsDeleteTexture(Texture_FireCount);
    gsDeleteTexture(Texture_FlyGauge);
    gsDeleteTexture(Texture_FlyGaugeEmpty);
    gsDeleteTexture(Texture_HealthIcon);
    gsDeleteTexture(Texture_Number);
    gsDeleteTexture(Texture_Coin);
    gsDeleteTexture(Texture_Slash);
    gsDeleteTexture(Texture_menu);
    gsDeleteTexture(Texture_ArrowIcon);
    gsDeleteTexture(Texture_Confirm_Window);
    gsDeleteTexture(Texture_Pause);
    gsDeleteTexture(Texture_Option);
    gsDeleteTexture(Texture_SoundGauge);
    gsDeleteTexture(Texture_SoundGaugeEmpty);
    gsDeleteTexture(Texture_kaihatu);

    gsDeleteBGM(BGM_GamePlayBGM);
    gsDeleteSE(SE_Select);
    gsDeleteSE(SE_WalkSound);
    gsDeleteSE(SE_Jump);
}