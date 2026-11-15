// Unit 307: midpoint of two paired f32 triples. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
    UnkVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3(const UnkVec3& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkBounds {
    u8 pad_0x0[0x8];
    f32 field_0x8, field_0xc;
    f32 field_0x10, field_0x14;
    f32 field_0x18, field_0x1c;
};

static inline f32 Mid(f32 a, f32 b) { return (a + b) * 0.5f; }

UnkVec3 fn_7100215ef4(const UnkBounds* self) {
    f32 x = Mid(self->field_0x8, self->field_0xc);
    f32 y = Mid(self->field_0x10, self->field_0x14);
    f32 z = Mid(self->field_0x18, self->field_0x1c);
    return UnkVec3(x, y, z);
}
