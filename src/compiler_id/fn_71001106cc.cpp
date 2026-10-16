// Multiplies a 2D vector by the 2x2 matrix stored at +0x8 (rows at +0x8, +0x10).
// The result type is returned through x8, so it is not trivially copyable.
#include "types.h"

struct UnkVec2 {
    f32 x, y;
    UnkVec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2(const UnkVec2& other) : x(other.x), y(other.y) {}
};

struct UnkStruct_0x18 {
    u8 unk_0x0[0x8];
    f32 m00, m01;
    f32 m10, m11;

    UnkVec2 fn_71001106cc(const UnkVec2& v) const;
};

UnkVec2 UnkStruct_0x18::fn_71001106cc(const UnkVec2& v) const {
    return UnkVec2(v.x * m00 + v.y * m01, v.x * m10 + v.y * m11);
}
