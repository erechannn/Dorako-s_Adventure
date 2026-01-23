#include "TitleScene.h"
#include "../World/Field.h"
#include "../Camera/TestCamera.h"
#include "../Light/MainLight.h"
#include "../Tween/Tween.h"
#include "../Actor/ModelPlayer.h"
#include "../Stage/StageManager.h"
#include "../Assets.h"

#include <GSstandard_shader.h>
#include <imgui/imgui.h>

void TitleScene::start() {

    // デフォルトシェーダーの初期化
    gsInitDefaultShader();
    // 視錐台カリングを有効にする
    gsEnable(GS_FRUSTUM_CULLING);

    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");

    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");

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
    //ステージのロード
    gsLoadTexture(Texture_Skybox, "Assets/Skybox/CosmicSkybox.dds");
    gsLoadOctree(Octree_TestStage, "Assets/Stage/TestStage/TestStageOctree.oct");
    gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/TestStage/TestStageOctreeCollider.oct");
    //テクスチャのロード
	gsLoadTexture(Texture_TitleLogo, "Assets/Texture/TitleUI/TitleLogo.png");
	gsLoadTexture(Texture_TitleUi, "Assets/Texture/TitleUI/TitleUi.png");
    gsLoadTexture(Texture_Number, "Assets/Texture/NullTexture.png");
    //プレイヤーのメッシュのロード
    gsLoadSkinMesh(Mesh_Player, "Assets/Mesh/Player/DragonSpark.mshb");
    //サウンドのロード
    gsLoadSE(SE_StartSelect, "Assets/Sound/SE/StartSelectSE.wav", 1, GS_TRUE);
    gsLoadBGM(BGM_TitleBGM, "Assets/Sound/BGM/TitleBGM.oga",GS_TRUE);
    //サンプル用のステージに変更
    StageManager::get_instance().select_stage(4);
    //タイトル画面用のプレイヤーの標示
    world_.add_actor(new ModelPlayer{ &world_,{0.0f,0.0f,0.0f} });
    //カメラの追加
	world_.add_camera(new TestCamera{
			 &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    //フィールドの追加
	world_.add_field(new Field{ Octree_TestStage,Octree_TestStageCollider,Texture_Skybox });
    //ライトの追加
	world_.add_light(new Light{ &world_ });

    //シーンの初期化
	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "StageSelectScene";

    ui_position_ = { 583.0f,682.0f };
    //タイトル用のposition
    first_pos_ = { 422.0f,-169.0f };
    end_pos_ = { 422.0f,-120.0f };
    //Tweenを追加
    Tween::vector2(first_pos_, end_pos_, 60, [=](GSvector2 pos) {logo_position_ = pos; });

    //BGMの再生
    gsPlayBGM(BGM_TitleBGM);
}
void TitleScene::update(float delta_time) {
	world_.update(delta_time);

    //タイトルロゴを動かす
    tween_timer_ += delta_time;
    if (tween_timer_ % 60 == 0) {
        GSvector2 first_pos = end_pos_;
        GSvector2 end_pos = first_pos_;
        Tween::vector2(first_pos, end_pos, 60, [=](GSvector2 pos) {logo_position_ = pos; });
        first_pos_ = first_pos;
        end_pos_ = end_pos;

    }

    if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) { 
        is_start_ = true;
        gsPlaySE(SE_StartSelect);
    }
    if (gsGetKeyTrigger(GKEY_SPACE)) {
        is_start_ = true;
        gsPlaySE(SE_StartSelect);
    }
	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了

	//if (ImGui::Begin("logo")) {
	//	ImGui::DragFloat2("LogoPosition:", logo_position_, 0.1f);
	//	ImGui::DragFloat2("UIPosition:", ui_position_, 0.1f);
	//	ImGui::End();
	//}
}
void TitleScene::draw()const {
	world_.draw();
	const GSvector2 Logo_position{ 0.0f,0.0f };
	gsDrawSprite2D(Texture_TitleLogo, &logo_position_, NULL, NULL, NULL, NULL, 0.0f);
    gsDrawSprite2D(Texture_TitleUi, &ui_position_, NULL, NULL, NULL, NULL, 0.0f);

}
bool TitleScene::is_end() const{
	return is_end_;
}
std::string TitleScene::next()const {
	return next_scene_;
}
void TitleScene::end() {
    //タイトル用のプレイヤーを死亡させる
    Actor*model_player=world_.find_actor("ModelPlayer");
    model_player->die();
    //ワールドの削除
    world_.clear();
    //実行しているTweenの停止
    Tween::clear();
    //BGMの停止
    gsStopBGM();
    //ロードしたものの削除
    gsDeleteShader(Shader_Silhouette);
    gsDeleteMesh(Mesh_Player);
    gsDeleteTexture(Texture_Skybox);
    gsDeleteTexture(Texture_TitleLogo);
    gsDeleteTexture(Texture_TitleUi);
    gsDeleteTexture(Texture_Number);
    gsDeleteOctree(Octree_TestStage);
    gsDeleteOctree(Octree_TestStageCollider);
    gsDeleteBGM(BGM_TitleBGM);
    gsDeleteSE(SE_StartSelect);
}