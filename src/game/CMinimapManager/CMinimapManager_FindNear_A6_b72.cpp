// CMinimapManager unit (tools/tu_map.py): the first 0x118-byte entry of a list (data +0x6c0, count +0x6d0 in a
// block at +0xd63d30) whose point at +0x24 lies within a Manhattan distance of 200 of the block's rectangle
// centre. Names are placeholders.
#include <math.h>
#include "types.h"

struct UnkMinimapEntry_A6_b72 {
    char pad_0x0[0x24];
    f32 x;
    f32 y;
    char pad_0x2c[0x118 - 0x2c];
};

struct UnkMinimapBlock_A6_b72 {
    f32 left;
    f32 top;
    f32 right;
    f32 bottom;
    char pad_0x10[0x6c0 - 0x10];
    UnkMinimapEntry_A6_b72* data;
    u64 field_0x6c8;
    u32 count;
};

UnkMinimapEntry_A6_b72* fn_7100ea61c4(char* self) {
    UnkMinimapBlock_A6_b72* b = reinterpret_cast<UnkMinimapBlock_A6_b72*>(self + 0xd63d30);
    asm("" : "+r"(b));
    u32 n = b->count;
    if (n == 0)
        return nullptr;
    f32 cx = (b->left + b->right) * 0.5f;
    f32 cy = (b->top + b->bottom) * 0.5f;
    UnkMinimapEntry_A6_b72* e = b->data;
    for (UnkMinimapEntry_A6_b72* end = e + n; e != end; ++e) {
        if (fabsf(e->x - cx) + fabsf(e->y - cy) < 200.0f)
            return e;
    }
    return nullptr;
}
