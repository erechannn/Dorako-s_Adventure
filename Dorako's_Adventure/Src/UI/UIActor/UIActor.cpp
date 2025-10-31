#include "UIActor.h"

// �L���ɂ���
void UIActor::enable() {
    enabled_ = true;
    on_enable();
}

// �����ɂ���
void UIActor::disable() {
    enabled_ = false;
    on_disable();
}

// �L���ɂȂ��Ă��邩
bool UIActor::get_enabled() const {
    return enabled_;
}

// �L��/������ݒ肷��
bool UIActor::set_enabled(bool enabled) {
    enabled_ = enabled;
    return enabled_;
}
