#include"GamePlayScene.h"
#include <GSstandard_shader.h>

void GamePlayScene::start() {

    // �f�t�H���g�V�F�[�_�[�̏�����
    gsInitDefaultShader();
    // ������J�����O��L���ɂ���
    gsEnable(GS_FRUSTUM_CULLING);

    // �V���h�E�}�b�v�̍쐬
    static const GSuint shadow_map_size[] = { 2048, 2048 };
    gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
    // �V���h�E�}�b�v��K�p���鋗��(���_����̋����j
    gsSetShadowMapDistance(60.0f);
    // �J�X�P�[�h�V���h�E�}�b�v�̕����ʒu�𒲐��i�f�t�H���g��0.5�j
    gsSetShadowMapCascadeLamda(0.7f);
    // �V���h�E�̔Z����ݒ�(0.0:�Z���`1.0:����)
    gsSetShadowMapAttenuation(0.0f);


	is_end_ = false;
	is_start_ = false;
	start_timer_ = 0.0f;
	next_scene_ = "StageSelectScene";

}