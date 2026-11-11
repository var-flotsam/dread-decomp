#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkVec3_ovb0245 {
    f32 x, y, z;
    UnkVec3_ovb0245(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3_ovb0245() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct UnkView_ovb0245 {
    u8 pad_0x0[0x30];
    UnkVec3_ovb0245 position;  // 0x30
    UnkVec3_ovb0245 scale;     // 0x3c
    u8 pad_0x48[0x11c];
    UnkMatrix44 matrix;  // 0x164
    u8 pad_0x1a4[8];
    u8 matrix_valid;  // 0x1ac
};
static_assert(__builtin_offsetof(UnkView_ovb0245, matrix_valid) == 0x1ac, "layout");

struct UnkSize_ovb0245 {
    u8 pad_0x0[8];
    u32 width;   // 0x8
    u32 height;  // 0xc
};

UnkMatrix44 fn_71000791a8(const UnkVec3_ovb0245* position, const UnkVec3_ovb0245& rotation, const UnkVec3_ovb0245* scale);
void fn_710015bf40(UnkMatrix44*, f32);
f32 fn_71000768c0(f32 x);
f32 fn_71000768f0(f32 y);

static inline bool Differs(f32 current, f32 value) {
    f32 low = current - 0.001f;
    f32 high = current + 0.001f;
    return !(high >= value) || !(low <= value);
}

UnkMatrix44 fn_710019da10(UnkView_ovb0245* self, UnkSize_ovb0245* size, f32 x, f32 y) {
    UnkMatrix44 result;
    __builtin_memset(&result, 0, sizeof(result));
    result.m[15] = 1.0f;
    result.m[10] = 1.0f;
    result.m[5] = 1.0f;
    result.m[0] = 1.0f;
    f32 width = (f32)size->width;
    f32 height = (f32)size->height;
    f32 aspect = width / height;
    UnkVec3_ovb0245 base = self->position;
    f32 py = y / aspect / height;
    py += base.y;
    UnkVec3_ovb0245 position(x / width + base.x, py, base.z);
    UnkMatrix44 m = fn_71000791a8(&position, UnkVec3_ovb0245(), &self->scale);
    UnkCopyMatrix(&result, m);
    fn_710015bf40(&result, aspect);
    return result;
}
