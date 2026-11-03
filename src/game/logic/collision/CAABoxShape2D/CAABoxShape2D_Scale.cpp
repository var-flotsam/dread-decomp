// game::logic::collision::CAABoxShape2D (unit 2511 of tools/tu_map.py, the name from reflection):
// scales the box (+0xa8 min, +0xb0 max) by a 2D factor about its centre, then lets the shape
// update (vtable slot 0x80). The factor is read as plain floats, so it is reread after each store.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBoxVec2 {
    f32 x, y;
};
static inline UnkBoxVec2 operator+(UnkBoxVec2 a, UnkBoxVec2 b) { return {a.x + b.x, a.y + b.y}; }
static inline UnkBoxVec2 operator-(UnkBoxVec2 a, UnkBoxVec2 b) { return {a.x - b.x, a.y - b.y}; }
static inline UnkBoxVec2 operator*(UnkBoxVec2 a, f32 s) { return {a.x * s, a.y * s}; }

struct UnkScaledBoxShape2D {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkBoxVec2 min;  // 0xa8
    UnkBoxVec2 max;  // 0xb0

    void vf_0x80() { reinterpret_cast<void (*)(UnkScaledBoxShape2D*)>(vtable[0x80 / 8])(this); }
};

void fn_7100d5e440(UnkScaledBoxShape2D* self, const f32* scale) {
    UnkBoxVec2 centre = (self->min + self->max) * 0.5f;
    self->min = self->min - centre;
    self->max = self->max - centre;
    self->min.x *= scale[0];
    self->min.y *= scale[1];
    self->max.x *= scale[0];
    self->max.y *= scale[1];
    self->min = centre + self->min;
    self->max = centre + self->max;
    self->vf_0x80();
}
