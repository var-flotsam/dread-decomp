// From unit 107 of tools/tu_map.py (0x710010d604-0x7100110c54, no reflected class; the 2D shapes of
// src/engine/SystemStartup.cpp): rotates a vector by a shape's angle, the sum of its two angles at
// +0x20 and +0x24. The vector type has a user-provided copy, so it is returned in memory (x8).
// Names and layouts are placeholders.
#include "types.h"

extern "C" f32 cosf(f32 x);
extern "C" f32 sinf(f32 x);

struct UnkVec2f {
    f32 x, y;
    UnkVec2f() {}
    UnkVec2f(f32 x, f32 y) : x(x), y(y) {}
    UnkVec2f(const UnkVec2f& o) : x(o.x), y(o.y) {}
};

struct UnkRotatedShape2D {
    u8 pad_0x0[0x20];
    f32 angle;   // 0x20
    f32 offset;  // 0x24
    f32 Angle() const { return offset + angle; }
};

UnkVec2f fn_71001106f8(const UnkRotatedShape2D* shape, const UnkVec2f& v) {
    f32 c0 = cosf(shape->Angle());
    f32 s0 = sinf(shape->Angle());
    f32 s1 = sinf(shape->Angle());
    f32 c1 = cosf(shape->Angle());
    return UnkVec2f(c0 * v.x - s0 * v.y, s1 * v.x + c1 * v.y);
}
