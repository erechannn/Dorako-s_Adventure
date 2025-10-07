#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Actor.h"
#include "../State/StateMachine.h"

class Character :public Actor {
public:
	//�R���g���X�g
	Character(GSuint mesh);
	//�X�V����
	virtual void update(float delta_time)override {}
	//�`�揈��
	virtual void draw() const override {}
	//�����蔻��
	virtual void react(Actor& other) override{}
	//��Ԃ̕ύX
	void change_state(int next_state);
public:
	bool is_ground();
protected:
	//�X�e�[�g�}�V��
	StateMachine state_;
	//�ǂƂ̓����蔻��
	void collide_field();
	//���A�N�^�[�Ƃ̏Փˏ���
	void collide_actor(Actor& other);
	//�d��
	void gravity_update(float delta_time);
	//�n�ʂƂ̓����蔻�菈��
	void collide_ground();
	//�d�͗�
	float gravity_{ -0.003 };
	//�d�͂ɂ��ړ���
	GSvector3 gravity_velocity_{ 0.0f,0.0f,0.0f };
	int max_health_{ 1 };
	int health_{ 1 };
	//�����̃I�t�Z�b�g
	float foot_offset_{ 0.1f };
	//�L�����̍���
	float height_{ 1.0f };
	//�d�͂�K�����邩
	bool is_ground_{ false };
	bool is_zero_gravity_{ false };
};



#endif // !CHARACTER_H_
