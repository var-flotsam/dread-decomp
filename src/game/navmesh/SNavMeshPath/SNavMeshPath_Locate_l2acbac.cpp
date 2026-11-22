// SNavMeshPath unit: walks the path's segments (polymorphic elements, slot 3 answers a position) and finds
// the segment a distance along the path falls in, with the fraction along it. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkVec3.h"
#include "types.h"

struct UnkNavElement_l2acbac {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3* vf_0x18();
    u8 pad_0x8[0x118];
};

#pragma pack(push, 4)
struct UnkNavMeshPath_l2acbac {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    UnkNavElement_l2acbac elements[2];  // 0xc
};
#pragma pack(pop)

void fn_71002acbac(UnkNavMeshPath_l2acbac* self, f32 distance, u32* segment, f32* fraction) {
    *segment = 0;
    *fraction = 0.0f;
    if (self->count < 2)
        return;
    f32 target = distance + -1.1920929e-7f;
    f32 walked = 0.0f;
    for (u64 i = 0; walked < target;) {
        const UnkVec3* to = self->elements[i + 1].vf_0x18();
        const UnkVec3* from = self->elements[i].vf_0x18();
        f32 dx = to->x - from->x;
        f32 dy = to->y - from->y;
        f32 dz = to->z - from->z;
        f32 length = sqrtf(dx * dx + dy * dy + dz * dz);
        if (!(walked + length < target)) {
            *fraction = (distance - walked) / length;
            *segment = i;
            return;
        }
        walked += length;
        *segment = i;
        *fraction = 1.0f;
        ++i;
        if (!(i + 1 < self->count))
            return;
    }
}
