// Unit 294 of tools/tu_map.py (no name hint): whether a point lies inside all six planes of a plane set
// (the given one, else the owner's), starting from the plane that rejected it last time and remembering
// the next one to test. Names and layouts are placeholders.
#include "types.h"

struct UnkPlane_l20eeb8 {
    s32 nx;
    s32 ny;
    s32 nz;
    s32 pad_0xc;
    s64 d;
};
static_assert(sizeof(UnkPlane_l20eeb8) == 0x18, "layout");

struct UnkPlaneSet_l20eeb8 {
    u8 pad_0x0[0x1d8];
    UnkPlane_l20eeb8 planes[6];  // 0x1d8
};

struct UnkPlaneOwner_l20eeb8 {
    u8 pad_0x0[0x108];
    UnkPlaneSet_l20eeb8* planes;  // 0x108
};

struct UnkPlaneTester_l20eeb8 {
    void* vtable;
    UnkPlaneOwner_l20eeb8* owner;  // 0x8
};

struct UnkBounds_l20eeb8 {
    u8 pad_0x0[0x38];
    s64 limit;  // 0x38
    s32 x;      // 0x40
    s32 y;      // 0x44
    s32 z;      // 0x48
    u8 pad_0x4c;
    u8 plane;   // 0x4d
};

bool fn_710020eeb8(UnkPlaneTester_l20eeb8* self, UnkBounds_l20eeb8* b, UnkPlaneSet_l20eeb8* set) {
    if (!set)
        set = self->owner->planes;
    // The point and the limit are read once; the plane index is stored back as it advances.
    const s32 x = b->x;
    const s32 y = b->y;
    const s32 z = b->z;
    const s64 limit = b->limit;
    u8 plane = b->plane;
    for (u32 i = 0; i < 6; ++i) {
        const UnkPlane_l20eeb8& p = set->planes[plane];
        s32 dot = x * p.nx + y * p.ny + z * p.nz;
        if (p.d + dot > limit)
            return false;
        u8 next = plane + 1;
        if (next == 6)
            next = 0;
        plane = next;
        b->plane = plane;
    }
    return true;
}
