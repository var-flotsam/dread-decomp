#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "game/UnkTU178.h"
#include "game/UnkTU53.h"

struct UnkView_ovb0245 {
    u8 pad_0x0[0x30];
    UnkVec3f position;  // 0x30
    UnkVec3f scale;     // 0x3c
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


static inline bool Differs(f32 current, f32 value) {
    f32 low = current - 0.001f;
    f32 high = current + 0.001f;
    return !(high >= value) || !(low <= value);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
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
    UnkVec3f base = self->position;
    f32 py = y / aspect / height;
    py += base.y;
    UnkVec3f position(x / width + base.x, py, base.z);
    UnkMatrix44 m = fn_71000791a8(position, UnkVec3f(0.0f, 0.0f, 0.0f), self->scale);
    UnkCopyMatrix(&result, m);
    fn_710015bf40(&result, aspect);
    return result;
}
