#include "SceneManager.h"
#include "SceneNull.h"
#include<GSinput.h>
#include<imgui/imgui.h>

static SceneNull scene_null; //null�V�[���i�������Ȃ��_�~�[�V�[���j

//�R���X�g���N�^
SceneManager::SceneManager() :
	current_scene_{ &scene_null } {
}

//�f�X�g���N�^
SceneManager::~SceneManager() {
	clear();
}

//�X�V
void SceneManager::update(float delta_time) {
	//�V�[���̍X�V
	current_scene_->update(delta_time);
	//�V�[�����I�����Ă��邩�H
	if (current_scene_->is_end()) {
		//�V�[����ύX����
		change(current_scene_->next());
	}

#ifdef _DEBUG
	constexpr bool IS_DEBUG_BUILD{ true };
#else
	constexpr bool IS_DEBUG_BUILD{ false };
#endif //_DEBUG

	if (IS_DEBUG_BUILD && gsGetKeyTrigger(GKEY_0)) {
		changer_display_flag_ = !changer_display_flag_;
	}
	if (changer_display_flag_) {
		ImGui::Begin("SceneChanger");
		for (auto& pair : scenes_) {
			if (ImGui::Button(pair.first.c_str())) {
				change(pair.first);
			}
		}
		ImGui::End();
	}
}

//�`��
void SceneManager::draw() const {
	//���݂̃V�[����`��
	current_scene_->draw();
}

//�I��
void SceneManager::end() {
	//���݂̃V�[�����I��
	current_scene_->end();
	//null�V�[���ɂ��Ă���
	current_scene_ = &scene_null;
}

void SceneManager::debug(float delta_time)
{
	current_scene_->debug(delta_time);
}

//�V�[���̒ǉ�
void SceneManager::add(const std::string& name, IScene* scene) {
	scenes_[name] = scene;
}

//�V�[���̕ύX
void SceneManager::change(const std::string& name) {
	//���݂̃V�[�����I��
	end();
	//���݂̃V�[����ύX
	current_scene_ = scenes_[name];
	//���݂̃V�[�����J�n
	current_scene_->start();
}

//�V�[���̏���
void SceneManager::clear() {
	//�O�̂��߃V�[���̏I�����Ăяo��
	end();
	//unordered_map���̃V�[�������ׂč폜
	for (auto& pair : scenes_) {
		delete pair.second; //�V�[���̍폜
	}
	//unordered_map������
	scenes_.clear();
}
