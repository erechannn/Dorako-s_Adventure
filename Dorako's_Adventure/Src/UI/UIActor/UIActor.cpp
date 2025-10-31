#include "UIActor.h"

// —LŒø‚É‚·‚é
void UIActor::enable() {
    enabled_ = true;
    on_enable();
}

// –³Œø‚É‚·‚é
void UIActor::disable() {
    enabled_ = false;
    on_disable();
}

// —LŒø‚É‚È‚Á‚Ä‚¢‚é‚©
bool UIActor::get_enabled() const {
    return enabled_;
}

// —LŒø/–³Œø‚ğİ’è‚·‚é
bool UIActor::set_enabled(bool enabled) {
    enabled_ = enabled;
    return enabled_;
}
