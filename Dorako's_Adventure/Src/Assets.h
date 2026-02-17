#ifndef ASSETS_H_
#define ASSETS_H_

//メッシュ
enum {
	Mesh_Player,
	Mesh_Planet,
	Mesh_Kuribo,
	Mesh_WoodBox,
	Mesh_MiniDragon,
	Mesh_Coin,
	Mesh_Door,
	Mesh_Bee,

};
//テクスチャ
enum {
	Texture_Skybox,
	Texture_TitleLogo,
	Texture_TitleUi,
	Texture_FireCount,
	Texture_EmptyFireCount,
	Texture_HealthIcon,
	Texture_EmptyHealth,
	Texture_StageClear,
	Texture_StageSelect,
	Texture_GameOver,
	Texture_FlyGaugeEmpty,
	Texture_FlyGauge,
	Texture_ArrowIcon,
	Texture_Number,
	Texture_BackGround,
	Texture_Coin,
	Texture_Slash,
	Texture_StageStartText,
	Texture_menu,
	Texture_Pause,
	Texture_Confirm_Window,
	Texture_Option,
	Texture_SoundGauge,
	Texture_SoundGaugeEmpty,
	Texture_kaihatu,
};
//オクツリー
enum {
	Octree_TestStage,
	Octree_TestStageCollider,
	Octree_Stage1,
	Octree_Stage1Collider,
	Octree_BossStage,
	Octree_BossStageCollider,
};
//エフェクト
enum {
	Effect_FireBoll,
	Effect_Teleportation,
	Effect_GetCoin,
};
//BGM
enum {
	BGM_TitleBGM,
	BGM_StageSelectBGM,
	BGM_GamePlayBGM,
};
//SE
enum {
	SE_StartSelect,
	SE_Select,
	SE_WalkSound,
	SE_Jump,
	SE_DamageEnemy

};
// シェーダー
enum {
	Shader_Silhouette               // シルエット用のシェーダー
};

// レンダーターゲット
enum {
	Rt_BaseScene,                   // メインシーン描画用
	Rt_Field,                       // 背景描画用
	Rt_Silhouette,                  // シルエット描画用
	Rt_Detpth                       // デプスバッファ用
};






#endif // !ASSETS_H_
