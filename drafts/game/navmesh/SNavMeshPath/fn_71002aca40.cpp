// SNavMeshPath unit: the path length up to segment `segment` (whole segments before it) plus the given
// fraction of that segment. Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

struct UnkNavElement_l2aca40 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3* vf_0x18();
    u8 pad_0x8[0x118];
};

#pragma pack(push, 4)
struct UnkNavMeshPath_l2aca40 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    UnkNavElement_l2aca40 elements[2];  // 0xc
};
#pragma pack(pop)

static inline f32 SegmentLength(UnkNavMeshPath_l2aca40* self, u32 i, u32 next) {
    const UnkVec3* to = self->elements[next].vf_0x18();
    const UnkVec3* from = self->elements[i].vf_0x18();
    f32 dx = to->x - from->x;
    f32 dy = to->y - from->y;
    f32 dz = to->z - from->z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

f32 fn_71002aca40(UnkNavMeshPath_l2aca40* self, s32 segment, f32 fraction) {
    f32 total = 0.0f;
    if (segment < 0)
        return total;
    s32 next = segment + 1;
    for (s32 j = 1; j < next; ++j)
        total += SegmentLength(self, j - 1, j);
    if (next < (s32)self->count)
        total += SegmentLength(self, segment, next) * fraction;
    return total;
}
