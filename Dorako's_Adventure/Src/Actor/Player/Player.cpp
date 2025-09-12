#include"Player.h"
#include"Assets.h"
#include"World/IWorld.h"
#include"World/Field.h"
#include"../../Shape/Line.h"
#include"PlayerMotion.h"
#include"PlayerState/PlayerStateDamage.h"
#include"PlayerState/PlayerStateMove.h"
#include"PlayerState/PlayerStateJump.h"
#include<imgui/imgui.h>
#include<iostream>

const float PlayerHeight{ 1.0f };
const float PlayerRadius{ 0.5f };
const float FootOffset{ 0.001f };
const float WalkSpeed{ 0.15f };

Player::Player(IWorld* world, GSvector3 position) :
	Actor{ Mesh_Player } {
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0,PlayerHeight,0} };
	transform_.position(position);
	mesh_->transform(transform_.localToWorldMatrix());

	state_.add_state(PlayerState::StateDamage, new PlayerStateDamage(this));
	state_.add_state(PlayerState::StateMove, new PlayerStateMove(this));
	state_.add_state(PlayerState::StateJumpStart, new PlayerStateJump(this));
	state_.change_state(PlayerState::StateMove);

}
void Player::update(float delta_time) {
	state_.update(delta_time);
	//���b�V���̃��[�V�������X�V
	mesh_->update(delta_time);
	//���[���h�ϊ��s���ݒ�
	mesh_->transform(transform_.localToWorldMatrix());
	//�t�B�[���h�Ƃ̓����蔻��
	collide_field();
	//�d��
	GSvector3 planet_position{0.0f,-20.0f,0.0f};
	GSvector3 position = transform_.position();
	GSvector3 gravity = position - planet_position;
	GSvector3 gravity_normalize = gravity.normalize();
	gravity_vro += gravity_normalize*gravity_ * delta_time;
	transform_.translate(gravity_vro, GStransform::Space::World);
	//�v���C���[��n�ʂɑ΂��Đ����ɗ�������
	player_rotate(delta_time);
	//�v���C���[�̏�ԊǗ�
	if (state_.now_state_ == PlayerState::StateMove && gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_A)) {
		state_.change_state(PlayerState::StateJumpStart);
	}
	if (state_.now_state_ == PlayerState::StateMove || state_.now_state_ == PlayerState::StateJumpStart) {
		is_move_ = true;
	}
	else is_move_ = false;


	//�f�o�b�N�\��
	ImGui::Begin("Player");
	ImGui::Text("x:%f y:%f z:%f", transform_.position().x, transform_.position().y, transform_.position().z);
	ImGui::Text("x:%f y:%f z:%f", velocity_.x, velocity_.y, velocity_.z);

	ImGui::End();
}
void Player::draw()const {
	mesh_->draw();
}
void Player::react(Actor& other) {
}
void Player::move(float delta_time) {
	GSvector2 result;
	//���X�e�B�b�N�̎擾
	gsXBoxPadGetLeftAxis(0, &result);
	//�X�e�B�b�N�̈ړ��ʂ̐��l��
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);

	if (!is_move_)return;
	// �J�����̑O�����x�N�g�����擾
	//GSvector3 forward = world_->camera()->transform().forward();
	//forward.y = 0.0f;
	//GSvector3 right = world_->camera()->transform().right();
	//right.y = 0.0f;
	//�v���C���[�
	GSvector3 forward = transform_.forward();
	GSvector3 right = transform_.right();
	// �X�e�B�b�N�̈ړ��l����ړ��x�N�g�����v�Z
	GSvector3 velocity{ 0.0f, 0.0f, 0.0f };
	//�E�X�e�B�b�N�ňړ�
	if (result.y > 0.0f) velocity += forward;
	if (result.y < 0.0f) velocity -= forward;
	if (result.x < 0.0f) velocity -= right;
	if (result.x > 0.0f) velocity += right;
	velocity = velocity.normalized() * WalkSpeed * result_normalize * delta_time;
	// �ړ����ĂȂ���΃A�C�h�����
	GSint motion{ PlayerMotion::Idle };
	// �ړ����Ă��邩�H
	if (velocity.length() != 0.0f) {
		// �����̕��
		GSquaternion rotation =
			GSquaternion::rotateTowards(
				transform_.rotation(),
				GSquaternion::lookRotation(velocity), 10.0f * delta_time);
		transform_.rotation(rotation);
		if (result_normalize <= 0.3f) {
			motion = PlayerMotion::Walk;
		}
		else if (result_normalize <= 0.8) {
			motion = PlayerMotion::Run;
		}
	}

	player_rotate(delta_time);

	velocity = transform_.transformDirection(velocity);

	// �ړ��ʂ�xz���������X�V
	velocity_ = velocity;

	mesh_->change_motion(motion, true);

	//���s�ړ�����i���[���h��j
	transform_.translate(velocity_, GStransform::Space::World);

	
}
void Player::jump(float delta_time) {
	if (state_.now_state_ != PlayerState::StateJumpStart) return;
	

}void Player::flying(float delta_time) {

}
void Player::landing(float delta_time) {

}
void Player::attack() {

}
void Player::collide_field() {
	// �ǂƂ̏Փ˔���i���̂Ƃ̔���j
	GSvector3 center; // �����߂���̋��̂̒��S���W
	if (world_->field()->collide(collider(), &center)) {
		// y���W�͕ύX���Ȃ�
		center.y = transform_.position().y;
		// �␳��̍��W�ɕύX����
		transform_.position(center);
	}
	// �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
	GSvector3 position = transform_.position();
	GSvector3 down_direction = -transform_.up();
	float line_length = PlayerHeight * 0.5f + FootOffset;
	//Line line;
	//line.start = position + collider_.center;
	////line.end = position + (down_direction * line_length);
	//line.end = GSvector3{ 0.0f,-20.0f,0.0f };
	//GSvector3 intersect;  // �n�ʂƂ̌�_
	//if (world_->field()->collide(line, &intersect)) {
	//	// ��_�̈ʒu����y���W�̂ݕ␳����
	//	position = intersect;
	//	// ���W��ύX����
	//	transform_.position(position);
	//	// �d�͂�����������
	//	velocity_.y = 0.0f;
	//}

}
void Player::collide_actor(Actor& other) {
	// �����W���������W�����߂�
	GSvector3 position = transform_.position();
	position.y = 0.0f;
	GSvector3 target = other.transform().position();
	target.y = 0.0f;
	// ����Ƃ̋���
	float distance = GSvector3::distance(position, target);
	// �Փ˔��苅�̔��a���m�����������������߂�
	float length = collider_.radius + other.collider().radius;
	// �Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
	float overlap = length - distance;
	// �d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
	GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
	transform_.translate(v, GStransform::Space::World);
	// �t�B�[���h�Ƃ̏Փ˔���
	collide_field();

}
void Player::player_rotate(float delta_time) {
	// �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
	GSvector3 line_start = transform_.position() + collider_.center;
	GSvector3 down_direction = -transform_.up();
	float line_length = PlayerHeight * 0.5f + FootOffset;
	GSvector3 line_end = transform_.position() + (down_direction * line_length);
	GSvector3 collision_point;    // �Փ˂����n�ʂƂ̌�_
	GSplane ground_plane;         // �Փ˂����n�ʂ̕���
	if (gsOctreeCollisionLine(gsGetOctree(Octree_TestStageCollider),
		&line_start, &line_end, &collision_point, &ground_plane)) {
		// �Փ˂����ʒu�ɍ��W��␳����
		transform_.position(collision_point);
		//std::cout << "x:" << ground_plane.normal.x << "y:" << ground_plane.normal.y << "z:" << ground_plane.normal.z << std::endl;
		std::cout << "x:" << collision_point.x << "y:" << collision_point.y << "z:" << collision_point.z << std::endl;
		//�Ζʕ����̈ړ��ʂ����߂�
		GSvector3 slope_velocity = GSvector3{ 0.0f, velocity_.y, 0.0 } - ground_plane.normal * ground_plane.normal.y * velocity_.y;
		
		// �Ζʂɍ��킹�ăL�����N�^���X������
		//GSvector3 up = GSvector3::rotateTowards(transform_.up(), ground_plane.normal, 0.1f * delta_time, 0.0);
		GSvector3 planet_position{ 0.0f,-20.0f,0.0f };
		GSvector3 up = transform_.position() - planet_position;
		GSvector3 left = GSvector3::cross(up, transform_.forward());
		GSvector3 forward = GSvector3::cross(left, up);
		transform_.rotation(GSquaternion::lookRotation(forward, up));
	}

}