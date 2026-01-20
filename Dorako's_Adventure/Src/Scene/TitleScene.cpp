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
    const static float fog_end{ 1000.0f };
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

    gsLoadTexture(Texture_Skybox, "Assets/Skybox/CosmicSkybox.dds");
    gsLoadOctree(Octree_TestStage, "Assets/Stage/TestStage/TestStageOctree.oct");
    gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/TestStage/TestStageOctreeCollider.oct");

	gsLoadTexture(Texture_TitleLogo, "Assets/Texture/TitleUI/TitleLogo.png");
	gsLoadTexture(Texture_TitleUi, "Assets/Texture/TitleUI/TitleUi.png");
    gsLoadTexture(Texture_Number, "Assets/Texture/NullTexture.png");

    gsLoadSkinMesh(Mesh_Player, "Assets/Mesh/Player/DragonSpark.mshb");

    gsLoadSE(SE_StartSelect, "Assets/Sound/SE/StartSelectSE.wav", 1, GS_TRUE);
    gsLoadBGM(BGM_TitleBGM, "Assets/Sound/BGM/TitleBGM.oga",GS_TRUE);

    StageManager::get_instance().select_stage(4);

    world_.add_actor(new ModelPlayer{ &world_,{0.0f,0.0f,0.0f} });

	world_.add_camera(new TestCamera{
			 &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });

	world_.add_field(new Field{ Octree_TestStage,Octree_TestStageCollider,Texture_Skybox });
	world_.add_light(new Light{ &world_ });


	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "StageSelectScene";
    ui_position_ = { 583.0f,682.0f };
    first_pos_ = { 422.0f,-169.0f };
    end_pos_ = { 422.0f,-120.0f };
    Tween::vector2(first_pos_, end_pos_, 60, [=](GSvector2 pos) {logo_position_ = pos; });
    gsPlayBGM(BGM_TitleBGM);

}
void TitleScene::update(float delta_time) {
	world_.update(delta_time);

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
    Actor*model_player=world_.find_actor("ModelPlayer");
    model_player->die();
    Tween::clear();
    gsStopBGM();

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