// game::logic::collision::CAABoxShape2D (unit 2511 of tools/tu_map.py, the name from reflection):
// fn_7100d5e3dc scales the box (+0xa8 min, +0xb0 max) uniformly about its centre, then lets the shape
// update (vtable slot 0x80); the 2D-factor version is fn_7100d5e440 (CAABoxShape2D_Scale.cpp). Names and
// layouts are placeholders.
#include "types.h"

struct UnkUniformBoxVec2 {
    f32 x, y;
};
static inline UnkUniformBoxVec2 operator+(UnkUniformBoxVec2 a, UnkUniformBoxVec2 b) { return {a.x + b.x, a.y + b.y}; }
static inline UnkUniformBoxVec2 operator-(UnkUniformBoxVec2 a, UnkUniformBoxVec2 b) { return {a.x - b.x, a.y - b.y}; }
static inline UnkUniformBoxVec2 operator*(UnkUniformBoxVec2 a, f32 s) { return {a.x * s, a.y * s}; }

struct UnkUniformScaledBoxShape2D {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkUniformBoxVec2 min;  // 0xa8
    UnkUniformBoxVec2 max;  // 0xb0

    void vf_0x80() { reinterpret_cast<void (*)(UnkUniformScaledBoxShape2D*)>(vtable[0x80 / 8])(this); }
};

void fn_7100d5e3dc(UnkUniformScaledBoxShape2D* self, f32 scale) {
    UnkUniformBoxVec2 centre = (self->min + self->max) * 0.5f;
    UnkUniformBoxVec2 min = self->min - centre;
    UnkUniformBoxVec2 max = self->max - centre;
    min.x *= scale;
    min.y *= scale;
    max.x *= scale;
    max.y *= scale;
    self->min.x = centre.x + min.x;
    self->min.y = centre.y + min.y;
    self->max.x = centre.x + max.x;
    self->max.y = centre.y + max.y;
    self->vf_0x80();
}
