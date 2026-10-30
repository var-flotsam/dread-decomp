// game::logic::collision::CCapsuleShape2D's unit: moves the capsule's centre (+0xa8) by `delta`, then
// lets it update (vf_0x80). Names and layouts are placeholders.
#include "types.h"

struct UnkShapeDelta2D {
    f32 x;
    f32 y;
};

struct UnkCapsuleShape2D {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 x;  // 0xa8
    f32 y;  // 0xac

    void vf_0x80() { reinterpret_cast<void (*)(UnkCapsuleShape2D*)>(vtable[0x80 / 8])(this); }
};

void fn_7100d6025c(UnkCapsuleShape2D* self, const UnkShapeDelta2D* delta) {
    self->x = delta->x + self->x;
    self->y = delta->y + self->y;
    self->vf_0x80();
}
