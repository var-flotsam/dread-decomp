// CAbilityGhostAura unit: the point a given distance back along the ability's trail of recorded positions,
// starting from the owner's position. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkAbility;

struct UnkGhostAuraOwner_l313074 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkGhostAuraEntity_l313074 {
    u8 pad_0x0[0x30];
    UnkGhostAuraOwner_l313074* owner;  // 0x30
};

struct UnkGhostAuraTrailPoint_l313074 {
    UnkVec3f pos;
    u8 pad_0xc[0x8];
};

struct UnkGhostAuraTrail_l313074 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraEntity_l313074* entity;  // 0xc0
    u8 pad_0xc8[0x3c8 - 0xc8];
    UnkGhostAuraTrailPoint_l313074* points;  // 0x3c8
    u32 count;                               // 0x3d0
};

// Walks the trail from its newest point back; on each segment that reaches past `amount`, the result is the
// point on it that far from its newer end.
UnkVec3f fn_7100313074(UnkAbility* ability, f32 amount) {
    UnkGhostAuraTrail_l313074* trail = reinterpret_cast<UnkGhostAuraTrail_l313074*>(ability);
    UnkVec3f result = trail->entity->owner->xform.GetTranslation();
    u32 count = trail->count;
    if (count < 2)
        return result;
    f32 walked = 0.0f;
    for (u32 i = count - 1; i != 0; --i) {
        const UnkVec3f& older = trail->points[i - 1].pos;
        const UnkVec3f& newer = trail->points[i].pos;
        f32 next = walked + Distance(older, newer);
        if (next < amount) {
            walked = next;
            continue;
        }
        f32 remaining = amount - walked;
        result = newer + Normalized(older - newer) * remaining;
    }
    return result;
}
