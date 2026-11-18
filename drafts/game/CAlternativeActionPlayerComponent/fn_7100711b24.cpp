// CAlternativeActionPlayerComponent unit (tools/tu_map.py unit 1138): the update. While the owner's animation
// has a CCharClassAllowAlternativeActionsTrack, the entries' cooldowns run down; every half second (at once
// when the owner's state flags say so) the entries keep their current values and the abort request plays.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkAltRect_l711b24 {
    f32 v[4];
};

struct UnkAltState_l711b24 {
    u8 pad_0x0[0x9e4];
    u32 flags;  // 0x9e4
};

struct UnkAltOwner_l711b24 {
    u8 pad_0x0[0x1d8];
    UnkAltState_l711b24* state;  // 0x1d8
    u8 pad_0x1e0[0x220 - 0x1e0];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkAltActionUpdate_l711b24 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAltOwner_l711b24* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 timer;  // 0x40
    u8 pad_0x44[0x4];
    UnkAltRect_l711b24* rects;  // 0x48
    u8 pad_0x50[0x8];
    u32 count;  // 0x58

    bool vf_0x168() {
        return reinterpret_cast<bool (*)(UnkAltActionUpdate_l711b24*)>(vtable[0x168 / 8])(this);
    }
};

struct UnkAltActionAbort_ovb0115;
void fn_7100711c88(UnkAltActionAbort_ovb0115* self);

void fn_7100711b24(UnkAltActionUpdate_l711b24* self, f32 dt) {
    fn_71010c4234(reinterpret_cast<char*>(self));
    if (!fn_710117b348(self->owner->animation, fn_7100bf0c1c()))
        return;
    u32 count = self->count;
    for (UnkAltRect_l711b24 *rect = self->rects, *end = rect + count; rect != end; ++rect) {
        if (rect->v[2] > 0.0f) {
            f32 left = rect->v[2] - dt;
            rect->v[2] = !(left < 0.0f) ? left : 0.0f;
        }
    }
    bool now = false;
    if (UnkAltState_l711b24* state = self->owner->state) {
        if (state->flags & 0x780100) {
            now = true;
            self->timer = 0.0f;
        }
    }
    self->timer -= dt;
    if (self->timer > 0.0f)
        return;
    self->timer += 0.5f;
    if (!now && self->vf_0x168())
        return;
    for (UnkAltRect_l711b24 *rect = self->rects, *end = rect + self->count; rect != end; ++rect) {
        rect->v[2] = rect->v[0];
        rect->v[3] = rect->v[1];
    }
    fn_7100711c88(reinterpret_cast<UnkAltActionAbort_ovb0115*>(self));
}
