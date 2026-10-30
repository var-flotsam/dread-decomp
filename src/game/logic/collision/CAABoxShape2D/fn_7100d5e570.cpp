// game::logic::collision::CAABoxShape2D's unit: moves the box's two corners (+0xa8 and +0xb0) by `delta`, then lets it update
// (vf_0x80), as the capsule shape does; the delta is read as plain floats, so it is reread after
// each store (src/game/logic/collision/CCapsuleShape2D/fn_7100d6025c.cpp).
// Names and layouts are placeholders.
#include "types.h"

struct UnkBoxShape2D_7100d5e570 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 min_x;  // 0xa8
    f32 min_y;
    f32 max_x;  // 0xb0
    f32 max_y;

    void vf_0x80() { reinterpret_cast<void (*)(UnkBoxShape2D_7100d5e570*)>(vtable[0x80 / 8])(this); }
};

void fn_7100d5e570(UnkBoxShape2D_7100d5e570* self, const f32* delta) {
    self->min_x = delta[0] + self->min_x;
    self->min_y = delta[1] + self->min_y;
    self->max_x = delta[0] + self->max_x;
    self->max_y = delta[1] + self->max_y;
    self->vf_0x80();
}
