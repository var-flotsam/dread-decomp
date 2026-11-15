// SLogicPathNode unit: walks a list of 0x20-byte nodes (fourth virtual gives the position) and finds
// the segment in which a distance along the path ends: its start index and the fraction along it.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/SLogicPathNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkLogicPathNode_l059794 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual const CVector3D* vf_3();
    u8 pad_0x8[0x18];
};

struct UnkDashPath {
    u8 pad_0x0[0x8];
    UnkLogicPathNode_l059794* data;
    u32 count;
};

void fn_7101059794(f32 distance, UnkDashPath* path, u32* index, f32* fraction) {
    *index = 0;
    *fraction = 0.0f;
    if (path->count < 2)
        return;
    f32 target = distance + -1.1920929e-7f;
    if (!(target > 0.0f))
        return;
    f32 travelled = 0.0f;
    for (u64 i = 1; i < path->count && travelled < target; ++i) {
        const CVector3D* a = path->data[i].vf_3();
        const CVector3D* b = path->data[i - 1].vf_3();
        f32 dx = a->x - b->x;
        f32 dy = a->y - b->y;
        f32 dz = a->z - b->z;
        f32 length = sqrtf(dx * dx + dy * dy + dz * dz);
        f32 end = travelled + length;
        if (end >= target) {
            *fraction = (distance - travelled) / length;
            travelled = distance;
        } else {
            *index = (u32)i;
            travelled = end;
        }
    }
}
