// Unit 224: setting a screen object's scale (+0x3c), unless every coordinate is within 0.001 of
// the current one; a change clears a flag (+0x1ac). Names and layouts are placeholders inferred
// from the code.

#include "game/UnkXformNode.h"
#include "types.h"

struct UnkScreenObject_710019c9f4 {
    u8 pad_0x0[0x30];
    UnkVec3f position;
    UnkVec3f scale;
    u8 pad_0x48[0x164];
    bool field_0x1ac;
};
static_assert(__builtin_offsetof(UnkScreenObject_710019c9f4, field_0x1ac) == 0x1ac, "");

static inline bool Near(f32 a, f32 b) {
    return b >= a - 0.001f && b <= a + 0.001f;
}

static inline bool Near(const UnkVec3f& a, const UnkVec3f& b) {
    return Near(a.x, b.x) && Near(a.y, b.y) && Near(a.z, b.z);
}

void fn_710019c9f4(UnkScreenObject_710019c9f4* self, const UnkVec3f* scale) {
    if (Near(self->scale, *scale))
        return;
    self->field_0x1ac = false;
    self->scale = *scale;
}
