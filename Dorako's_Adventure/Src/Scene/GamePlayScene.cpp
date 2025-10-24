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

    // �f�t�H���g�V�F�[�_�[�̏�����
    gsInitDefaultShader();
    // ������J�����O��L���ɂ���
    gsEnable(GS_FRUSTUM_CULLING);

    //// �V���h�E�}�b�v�̍쐬
    //static const GSuint shadow_map_size[] = { 2048, 2048 };
    //gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    //// �V���h�E�}�b�v��K�p���鋗��(���_����̋����j
    //gsSetShadowMapDistance(60.0f);
    //// �J�X�P�[�h�V���h�E�}�b�v�̕����ʒu�𒲐��i�f�t�H���g��0.5�j
    //gsSetShadowMapCascadeLamda(0.7f);
    //// �V���h�E�̔Z����ݒ�(0.0:�Z���`1.0:����)
    //gsSetShadowMapAttenuation(0.0f);

    gsLoadLightmap(0, "Assets/Lightmap/Lightmap.txt");

    gsLoadReflectionProbe(0, "Assets/RefProbe/ReflectionProbe.txt");

    // �t�H�O�̐ݒ�
    const static float fog_color[4]{ 0.190f, 0.231f, 0.238f, 1.0f };// Fog�̐F R190  G231 B238
    const static float fog_start{ 10.0f };
    const static float fog_end{ 100.0f };
    glFogi(GL_FOG_MODE, GL_LINEAR);     // ���`�t�H�O
    glFogfv(GL_FOG_COLOR, fog_color);   // �t�H�O�̐F
    glFogf(GL_FOG_START, fog_start);    // �t�H�O�̊J�n�ʒu�i���_����̋����j
    glFogf(GL_FOG_END, fog_end);        // �t�H�O�̏I���ʒu�i���_����̋����j
    glEnable(GL_FOG);                   // �t�H�O��L���ɂ���

    //�f�t�H���g�V�F�[�_�[�̏������i���b�V���t�@�C����ǂݍ��ޑO�ɗL���ɂ���j
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
    // ������J�����O��L���ɂ���
    gsEnable(GS_FRUSTUM_CULLING);

}
void GamePlayScene::update(float delta_time) {
    // ���[���h�N���X�̍X�V
    world_.update(delta_time);
    if (is_start_)          start_timer_ += delta_time; //�^�C�}�[����
    if (start_timer_ >= 60.0f)          is_end_ = true; //�V�[�����I��

}
bool GamePlayScene::is_end()const {
    return is_end_;
}
std::string GamePlayScene::next()const {
    return "";
}
void GamePlayScene::draw()const {
    // ���[���h�̕`��
    world_.draw();
}
void GamePlayScene::end() {
    gsDeleteSkinMesh(Mesh_Player);
    world_.clear();
}