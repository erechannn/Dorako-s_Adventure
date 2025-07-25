#include"Player.h"
#include"Assets.h"
#include"World/IWorld.h"
#include"World/Field.h"
#include"Input/GamePadInput.h"
#include"PlayerMotion.h"
#include"PlayerState/PlayerStateDamage.h"

const float PlayerHeight{ 1.0f };
const float PlayerRadius{ 0.5f };
const float FootOffset{ 0.1f };
const float WalkSpeed{ 0.25f };

Player::Player(IWorld* world, GSvector3 position) :
	Actor{ Mesh_Player } {
	world_ = world;
	name_ = "Player";
	tag_ = "PlayerTag";
	collider_ = BoundingSphere{ PlayerRadius,GSvector3{0,PlayerHeight,0} };
	transform_.position(position);

	state_.add_state(PlayerState::StateDamage, new PlayerStateDamage(this));
}
void Player::update(float delta_time) {
	state_.update(delta_time);
}
void Player::draw()const {

}
void Player::react(Actor& other) {

}
GSvector2 Player::move_input() {
	return enable_collider_ ? GamePadInput::get_stick(PadVec2::LeftStick, 0) : GSvector2::zero();
}
void Player::move(float delta_time) {
	GSvector2 result;
	//���X�e�B�b�N�̎擾
	gsXBoxPadGetLeftAxis(0, &result);
	//�X�e�B�b�N�̈ړ��ʂ̐��l��
	float result_normalize = std::sqrt(result.x * result.x + result.y * result.y);

	if (state_.now_state_ != PlayerState::StateMove)return;
	// �J�����̑O�����x�N�g�����擾
	GSvector3 forward = world_->camera()->transform().forward();
	forward.y = 0.0f;
	GSvector3 right = world_->camera()->transform().right();
	right.y = 0.0f;
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
				GSquaternion::lookRotation(velocity), 100.0f * delta_time);
		transform_.rotation(rotation);
		if (result_normalize <= 0.3f) {
			motion = PlayerMotion::Walk;
		}
		else if (result_normalize <= 0.8) {
			motion = PlayerMotion::Run;
		}
	}

	// �ړ��ʂ�xz���������X�V
	velocity_.x = velocity.x;
	velocity_.z = velocity.z;



	//���s�ړ�����i���[���h��j
	transform_.translate(velocity_, GStransform::Space::World);
}
