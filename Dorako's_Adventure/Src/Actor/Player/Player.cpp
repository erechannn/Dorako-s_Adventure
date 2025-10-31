#include "Player.h"
#include "Assets.h"
#include "World/IWorld.h"
#include "World/Field.h"
#include "../../Shape/Line.h"
#include "../BulletClass.h"
#include "PlayerMotion.h"
#include "PlayerState/PlayerStateDamage.h"
#include "PlayerState/PlayerStateMove.h"
#include "PlayerState/PlayerStateJump.h"
#include "PlayerState/PlayerStateFlying.h"
#include "PlayerState/PlayerStateJumpEnd.h"
#include "PlayerState/PlayerStateDead.h"
#include <imgui/imgui.h>
#include <iostream>

const float PlayerHeight{ 1.0f };
const float PlayerRadius{ 0.5f };
const float WalkSpeed{ 0.15f };

Player::Player(IWorld* world, GSvector3 position) :
	Character{ Mesh_Player } {
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0,PlayerHeight,0} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());
	max_health_ = 300000;
	health_ = max_health_;
	//��Ԃ̒ǉ�
	state_.add_state(PlayerState::StateDamage, new PlayerStateDamage(this));
	state_.add_state(PlayerState::StateMove, new PlayerStateMove(this));
	state_.add_state(PlayerState::StateJumpStart, new PlayerStateJump(this));
	state_.add_state(PlayerState::StateFlying, new PlayerStateFlying(this));
	state_.add_state(PlayerState::StateJumpEnd, new PlayerStateJumpEnd(this));
	state_.add_state(PlayerState::StateDead, new PlayerStateDead(this));
	state_.change_state(PlayerState::StateMove);

}
void Player::update(float delta_time) {
	state_.update(delta_time);
	//���b�V���̃��[�V�������X�V
	mesh_->update(delta_time);
	//�t�B�[���h�Ƃ̓����蔻��
	collide_field();
	gravity_update(delta_time);
	//�v���C���[��n�ʂɑ΂��Đ����ɗ�������
	collide_ground();
	//���[���h�ϊ��s���ݒ�
	mesh_->transform(transform_.localToWorldMatrix());

	//�v���C���[�̏�ԊǗ�
	if (state_.now_state_ == PlayerState::StateMove && gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_A)) {	//A�{�^���ŃW�����v

		state_.change_state(PlayerState::StateJumpStart);
	}

	//�������Ԃ�
	if (state_.now_state_ == PlayerState::StateMove || state_.now_state_ == PlayerState::StateFlying||state_.now_state_==PlayerState::StateJumpStart) {
		is_move_ = true;
	}
	else is_move_ = false;
	if(gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_B)&&is_move_){
		GSvector3 position = transform_.position() + transform_.forward();
		world_->add_actor(new BulletClass{ world_,position,transform_.forward(),Effeck_FireBoll,"PlayerAttackTag","PlayerFireAttack",tag_});
	}


	//�f�o�b�N�\��
	ImGui::Begin("Player");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("gravity:%f", gravity_);
	ImGui::Checkbox("�n��ɂ��邩", &is_ground_);

	ImGui::End();
}
void Player::draw()const {
	//���b�V���̕\��
	mesh_->draw();
	GSvector3 up = transform_.up();
	std::cout << "2 x: " << up.x << " y: " << up.y << " z: " << up.z << std::endl;

}
//�����蔻��
void Player::react(Actor& other) {
	//���ɂȂ�
	if (other.tag() == "EnemyTag") {
		if (is_above_enemy(other)) {
			state_.change_state(PlayerState::StateJumpStart);
		}
		else {
			//�m�b�N�o�b�N
			GSvector3 now_pos{ transform().position() };
			GSvector3 knock_pos{ now_pos };
			knock_pos -= transform().forward() * 2.0f;
			// �␳��̍��W�ɕύX����
			transform_.position(knock_pos);
			health_ -= 1;
			state_.change_state(PlayerState::StateDamage);

		}
	}
}
void Player::move(float delta_time) {
	//�X�e�B�b�N��2�����x�N�g��
	GSvector2 result;
	//���X�e�B�b�N�̎擾
	gsXBoxPadGetLeftAxis(0, &result);
	//�X�e�B�b�N�̈ړ��ʂ̐��l��
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);
	//�������Ԃ�
	if (!is_move_)return;
	// �J�����̑O�����x�N�g�����擾
	GSvector3 forward = world_->camera()->transform().forward();
	forward = transform_.inverseTransformVector(forward);//���[���h���烍�[�J����
	forward.y = 0.0f;//Y�͖���
	forward = forward.normalize();//���K��
	//�J�����̉E�����x�N�g�����擾
	GSvector3 right = world_->camera()->transform().right();
	right = transform_.inverseTransformDirection(right);//���[���h���烍�[�J��
	right.y = 0.0f;//Y�͖���
	right = right.normalize();//���K��
	// �X�e�B�b�N�̈ړ��l����ړ��x�N�g�����v�Z
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	//�E�X�e�B�b�N�ňړ�
	//�O���ɓ|���đO�����ɉ��Z���ړ�
	if (result.y > 0.0f) velocity += forward;
	if (result.y < 0.0f) velocity -= forward;
	//�E���ɓ|���ĉE�����ɉ��Z���ړ�
	if (result.x < 0.0f) velocity -= right;
	if (result.x > 0.0f) velocity += right;
	//�ړ��ʂ�|�����ʂɂ���ăX�s�[�h�𒲐�
	velocity = velocity.normalized() * WalkSpeed  *result_normalize* delta_time;
	// �ړ����ĂȂ���΃A�C�h�����
	GSint motion{ PlayerMotion::Idle };
	// �ړ����Ă��邩�H
	if (velocity.length() != 0.0f) {
		// �ړ������Ɍ���
		GSvector3 velocity_world = transform_.transformDirection(velocity);//���[�J�����烏�[���h
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity_world,transform_.up()), 15.0f * delta_time);
		transform_.rotation(rotation);
		//�v���C���[�̈ړ��ʂɍ��킹�ă��[�V�����̕ω�
		if (result_normalize <= 0.2f) {
			motion = PlayerMotion::Walk;
		}
		else  {
			motion = PlayerMotion::Run;
		}
	}
	//�ړ��ʂ����[�J�����烏�[���h��
	velocity = transform_.transformDirection(velocity);
	if (!is_ground_) {
		motion = PlayerMotion::Flying;
		GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
		GSvector3 up = transform_.position() - planet_position;
		GSvector3 left = GSvector3::cross(up, transform_.forward());
		GSvector3 forward = GSvector3::cross(left, up);
		transform_.rotation(GSquaternion::lookRotation(forward, up));

	}
	// �ړ��ʂ�xz���������X�V
	velocity_ = velocity;
	if (state_.now_state_ != PlayerState::StateJumpStart) {
		//���[�V�����̕ύX
		mesh_->change_motion(motion, true);


	}
	//���s�ړ�����i���[���h��j
	transform_.translate(velocity, GStransform::Space::World);

	
}
void Player::flying(float delta_time) {
	if (gsXBoxPadButtonState(0, GS_XBOX_PAD_A)) {
		is_zero_gravity_ = true;
	}
	else is_zero_gravity_ = false;
	move(delta_time);
}
void Player::landing(float delta_time) {

}
void Player::attack() {

}
bool Player::is_motion_end() {
	return mesh_->is_end_motion();
}
bool Player::is_above_enemy(Actor&other) {
	GSvector3 position = transform_.position();
	GSvector3 other_position = other.transform().position();
	GSvector3 other_up = other.transform().up();
	GSvector3 to_other = position - other_position;
	float dot = to_other.dot(other_up);
	return dot > 0;
}