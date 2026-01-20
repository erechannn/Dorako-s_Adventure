#include "StageSelectScene.h"
#include "../Stage/StageManager.h"
#include "../Assets.h"
#include"../Camera/CameraRotateAround.h"
#include"../Camera/MainCamera.h"
#include "../Camera/TestCamera.h"
#include "../Camera/DummyPlayer.h"
#include"../World/Field.h"
#include"../Light/MainLight.h"
#include"../Actor/Player/Player.h"
#include "../Actor/StageSelectDoor.h"
#include <GSstandard_shader.h>
#include <GSeffect.h>

#include <imgui/imgui.h>



void StageSelectScene::start() {
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
	gsLoadOctree(Octree_TestStage, "Assets/Stage/SelectStage/SelectStageOctree.oct");
	gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/SelectStage/SelectStageOctreeCollider.oct");

	gsLoadSkinMesh(Mesh_Player, "Assets/Mesh/Player/DragonSpark.mshb");

	gsLoadTexture(Texture_StageSelect, "Assets/Texture/StageSelectUI/StageSelect.png");
	gsLoadTexture(Texture_StageStartText, "Assets/Texture/StageSelectUI/StageStratText.png");

	gsLoadBGM(BGM_StageSelectBGM, "Assets/Sound/BGM/StageSelectBGM.oga", GS_TRUE);
	gsLoadSE(SE_Select, "Assets/Sound/SE/SelectSE.wav", 1, GS_TRUE);
	gsLoadSE(SE_WalkSound, "Assets/Sound/SE/WalkSound.wav", 10, GS_TRUE);
	gsLoadSE(SE_Jump, "Assets/Sound/SE/JumpSE.wav", 1, GS_TRUE);

	gsLoadEffect(Effect_Teleportation, "Assets/Effect/MagicCircle01.efkefc");


	StageManager::get_instance().select_stage(4);

	world_.add_actor(new StageSelectDoor{ &world_,{0.0f,0.0f,0.0f},1 });

	world_.add_actor(new Player{ &world_, {0.0f,0.0f,-3.0f} });
	world_.add_actor(new DummyPlayer{ &world_ });

	world_.add_camera(new CameraRotateAround{
				 &world_, GSvector3{ 0.0f, 1.0f, 3.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });

	world_.add_field(new Field{ Octree_TestStage,Octree_TestStageCollider,Texture_Skybox });
	world_.add_light(new Light{ &world_ });

	max_stage_count_ = StageManager::get_instance().get_total_stage_count();


	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "GamePlayScene";
	stage_count_ = 1;
	arrow_icon_position_ = { 560.0f,382.0f };
	gsPlayBGM(BGM_StageSelectBGM);
}

void StageSelectScene::update(float delta_time) {
	if (!is_start_) {
		world_.update(delta_time);

		auto doors = world_.find_actor_with_tag("DoorTag");
		for (size_t i = 0; i < doors.size(); ++i) {
			StageSelectDoor* stage_select_door = (StageSelectDoor*)doors[i];
			if (stage_select_door->is_into_the_door()) {
				is_into_the_door_ = true;
				stage_count_ = stage_select_door->get_stage_id();
				break;
			}
			else {
				is_into_the_door_ = false;
			}
		}

		if (gsGetKeyTrigger(GKEY_1)) {
			stage_count_ = 1;
			is_start_ = true;
		}
		if (gsGetKeyTrigger(GKEY_2)) {
			stage_count_ = 2;
			is_start_ = true;
		}
		if (gsGetKeyTrigger(GKEY_3)) {
			stage_count_ = 3;
			is_start_ = true;
		}

		if (is_into_the_door_) {
			if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) {
				is_start_ = true;
				gsPlaySE(SE_Select);
			}
		}
	}

	if (is_start_)          start_timer_ += delta_time; //タイマー増加
	if (start_timer_ >= 60.0f)          is_end_ = true; //シーンを終了

	//if (ImGui::Begin("UI_position")) {
	//	ImGui::Checkbox("into_the_door", &is_into_the_door_);
	//	ImGui::DragFloat2("UIPosition:", position_, 0.1f);
	//	ImGui::End();
	//}

}

void StageSelectScene::draw()const {
	world_.draw();
	const GSvector2 stage_select_UI_position{ 546.0,-86.0f };
	const GSvector2 stage_start_text_position{ 1130.0f,600.0f };
	gsDrawSprite2D(Texture_StageSelect, &stage_select_UI_position, NULL, NULL, NULL, NULL, 0.0f);
	if (is_into_the_door_) {
		gsDrawSprite2D(Texture_StageStartText, &stage_start_text_position, NULL, NULL, NULL, NULL, 0.0f);
	}
}

bool StageSelectScene::is_end()const {
	return is_end_;
}

std::string StageSelectScene::next()const {
	return next_scene_;
}

void StageSelectScene::end() {
	gsStopAllEffects();
	world_.clear();
	StageManager::get_instance().select_stage(stage_count_);
	gsStopBGM();

	gsDeleteShader(Shader_Silhouette);
	gsDeleteBGM(BGM_StageSelectBGM);
	gsDeleteSE(SE_Select);
	gsDeleteSE(SE_WalkSound);
	gsDeleteSE(SE_Jump);
	gsDeleteMesh(Mesh_Player);
	gsDeleteOctree(Octree_TestStage);
	gsDeleteOctree(Octree_TestStageCollider);
	gsDeleteTexture(Texture_Skybox);
	gsDeleteEffect(Effect_Teleportation);
}