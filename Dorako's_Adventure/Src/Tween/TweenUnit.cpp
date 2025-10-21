#include "TweenUnit.h"
#include <algorithm>

TweenUnit::TweenUnit(float duration) : duration_{ duration } {}

void TweenUnit::update(float delta_time) {
    // �o�ߎ��Ԃ�i�߂�
    elapsed_ += delta_time;
    // �i�����i0�`1�j���v�Z
    float progress = elapsed_ / duration_;
    // �i������1�𒴂��Ȃ��悤�ɂ���
    progress = std::min(progress, 1.0f);
    // �h���N���X���Ƃ̍X�V�������s��
    on_update(progress);
    // �I���������𔻒肷��
    is_finished_ = progress >= 1.0f;
}

bool TweenUnit::is_finished() const {
    return is_finished_;
}

