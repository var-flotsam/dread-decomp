// CAbilityGhostAura unit: appends a position to the trail when it lies more than 0.1 from the last one,
// adding the step to the distance walked. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

// One recorded position, with the distance walked when it was recorded.
struct UnkGhostAuraTrailPoint_l308a90 {
    UnkVec3f pos;
    f32 walked;
    bool field_0x10;
    bool field_0x11;

    UnkGhostAuraTrailPoint_l308a90(const UnkVec3f& pos, f32 walked, bool flag)
        : pos(pos), walked(walked), field_0x10(flag), field_0x11(false) {}
};

using UnkGhostAuraTrail_l308a90 = base::global::CRntVector<UnkGhostAuraTrailPoint_l308a90>;

// CRntVector's append, growing to the next multiple of the step when full.
static void Append(UnkGhostAuraTrail_l308a90* trail, const UnkGhostAuraTrailPoint_l308a90& value) {
    u32 n = trail->count;
    u32 cap = trail->capacity;
    u32 new_capacity = cap;
    if (cap <= n && trail->step != 0)
        new_capacity = (n / trail->step + 1) * trail->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkGhostAuraTrailPoint_l308a90* p;
    if (cap < new_capacity) {
        p = static_cast<UnkGhostAuraTrailPoint_l308a90*>(
            trail->allocator->vf_0x40((u64)new_capacity * sizeof(UnkGhostAuraTrailPoint_l308a90), 8, trail->flags));
        if (trail->data) {
            UnkGhostAuraTrailPoint_l308a90* src = trail->data;
            UnkGhostAuraTrailPoint_l308a90* dst = p;
            for (u32 i = 0; i < trail->count; ++i, ++dst, ++src)
                new (dst) UnkGhostAuraTrailPoint_l308a90(*src);
            trail->allocator->vf_0x48(trail->data);
        }
        n = trail->count;
        trail->data = p;
        trail->capacity = new_capacity;
    } else {
        p = trail->data;
    }
    new (&p[n]) UnkGhostAuraTrailPoint_l308a90(value);
    ++trail->count;
}

bool fn_7100308a90(const UnkVec3f* pos, bool flag, f32* walked, UnkGhostAuraTrail_l308a90* trail) {
    if (trail->count == 0)
        return false;
    f32 step = Distance(trail->data[trail->count - 1].pos, *pos);
    if (!(step > 0.1f))
        return false;
    *walked += step;
    UnkGhostAuraTrailPoint_l308a90 point(*pos, *walked, flag);
    Append(trail, point);
    return true;
}
