#ifndef TWEEN_H_
#define TWEEN_H_

#include <list>
#include "TweenUnit.h"

// Tween���[�V�������Ǘ�����static�N���X�B
// 1��1�̓�����TweenUnit�N���X���s���B
// ���̃N���X�ł́A������Tween���[�V�����̊Ǘ����s���B
class Tween {
public:
    // TweenUnit��ǉ����ĊǗ����ɒu��
    static void add(TweenUnit* unit);
    // �Ǘ�����TweenUnit�̍X�V�������Ăяo��
    static void update(float delta_time);
    // �Ǘ�����TweenUnit��S�č폜����
    static void clear();
    // float�^�̒l��Tween����
    static TweenUnit& value(float from, float to, float duration, std::function<void(float)> update_func);

    static TweenUnit& vector3(const GSvector3& from, const GSvector3& to, float duration, std::function<void(const GSvector3&)> update_func);
    static TweenUnit& vector2(const GSvector2& from, const GSvector2& to, float duration, std::function<void(const GSvector2&)> update_func);

private:
    // �C���X�^���X�����֎~
    Tween() = delete;
    // ������TweenUnit���Ǘ����邽�߂̃��X�g
    static std::list<TweenUnit*> units_;
};

#endif

