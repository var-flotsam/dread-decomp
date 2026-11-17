#include "types.h"

struct UnkPlane_b0258g {
    s32 nx;
    s32 ny;
    s32 nz;
    s32 pad_0xc;
    s64 d;
};
static_assert(sizeof(UnkPlane_b0258g) == 0x18, "layout");

struct UnkPlaneSet_b0258g {
    u8 pad_0x0[0x1d8];
    UnkPlane_b0258g planes[6];  // 0x1d8
};

struct UnkBounds_b0258g {
    u8 pad_0x0[0x38];
    s64 limit;  // 0x38
    s32 x;      // 0x40
    s32 y;      // 0x44
    s32 z;      // 0x48
    bool always_inside;  // 0x4c
    u8 plane;   // 0x4d
};

bool fn_71001ba188(UnkPlaneSet_b0258g* self, UnkBounds_b0258g* b) {
    if (b->always_inside)
        return true;
    // The point and the limit are read once; the plane index is stored back as it advances.
    const s32 x = b->x;
    const s32 y = b->y;
    const s32 z = b->z;
    const s64 limit = b->limit;
    u8 plane = b->plane;
    for (u32 i = 0; i < 6; ++i) {
        const UnkPlane_b0258g& p = self->planes[plane];
        s32 dot = x * p.nx + y * p.ny + z * p.nz;
        if (p.d + dot >= limit)
            return false;
        u8 next = plane + 1;
        if (next == 6)
            next = 0;
        plane = next;
        b->plane = plane;
    }
    return true;
}
