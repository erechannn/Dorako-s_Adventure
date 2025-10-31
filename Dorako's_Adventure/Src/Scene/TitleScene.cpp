#include "TitleScene.h"
#include "../World/Field.h"
#include "../Camera/TestCamera.h"
#include "../Light/MainLight.h"
#include "../Tween/Tween.h"
#include "../Actor/ModelPlayer.h"
#include "../Assets.h"

#include <GSstandard_shader.h>
#include <imgui/imgui.h>

void TitleScene::start() {

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
    const static float fog_end{ 1000.0f };
    glFogi(GL_FOG_MODE, GL_LINEAR);     // ���`�t�H�O
    glFogfv(GL_FOG_COLOR, fog_color);   // �t�H�O�̐F
    glFogf(GL_FOG_START, fog_start);    // �t�H�O�̊J�n�ʒu�i���_����̋����j
    glFogf(GL_FOG_END, fog_end);        // �t�H�O�̏I���ʒu�i���_����̋����j
    glEnable(GL_FOG);                   // �t�H�O��L���ɂ���

    //�f�t�H���g�V�F�[�_�[�̏������i���b�V���t�@�C����ǂݍ��ޑO�ɗL���ɂ���j
    gsInitDefaultShader();

    gsLoadTexture(Texture_Skybox, "Assets/Skybox/TestStageSkybox.dds");
    gsLoadOctree(Octree_TestStage, "Assets/Stage/testStage2.oct");
    gsLoadOctree(Octree_TestStageCollider, "Assets/Stage/testStage2Collider.oct");

	gsLoadTexture(Texture_TitleLogo, "Assets/Texture/TitleUI/TitleLogo.png");
	gsLoadTexture(Texture_TitleUi, "Assets/Texture/TitleUI/TitleUi.png");

    gsLoadSkinMesh(Mesh_Player, "Assets/Mesh/Player/DragonSpark.mshb");

    world_.add_actor(new ModelPlayer{ &world_,{0.0f,0.0f,0.0f} });

	world_.add_camera(new TestCamera{
			 &world_, GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });

	world_.add_field(new Field{ Octree_TestStage,Octree_TestStageCollider,Texture_Skybox });
	world_.add_light(new Light{ &world_ });


	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "GamePlayScene";
    ui_position_ = { 583.0f,682.0f };
    first_pos_ = { 422.0f,-169.0f };
    end_pos_ = { 422.0f,-120.0f };
    Tween::vector2(first_pos_, end_pos_, 60, [=](GSvector2 pos) {logo_position_ = pos; });

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

	if (gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A)) is_start_ = true;
	if (is_start_)          start_timer_ += delta_time; //�^�C�}�[����
	if (start_timer_ >= 60.0f)          is_end_ = true; //�V�[�����I��

	if (ImGui::Begin("logo")) {
		ImGui::DragFloat2("LogoPosition:", logo_position_, 0.1f);
		ImGui::DragFloat2("UIPosition:", ui_position_, 0.1f);
		ImGui::End();
	}
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
    Tween::clear();
}