// CDemolitionBlockLifeComponent's unit (tools/tu_map.py, unit 1374): the update. After the base life update,
// while the colour differs from its goal, steps it (fn_71008c6b0c) and gives it to the target object (and to
// the target's tint at +0x2e0, when there is one). Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/CDemolitionBlockLifeComponent.h"
#include "game/CLifeComponent.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkCommanderLife_A5_b63;
struct UnkColorLerp_A4_b75;

struct UnkDemolitionLifeTint {
    u8 pad_0x0[0x30];
    CVector4D* colour;  // 0x30
};

// The object the colour is applied to.
struct UnkDemolitionLifeTarget {
    u8 pad_0x0[0x240];
    CVector4D colour;  // 0x240
    u8 pad_0x250[0x90];
    UnkDemolitionLifeTint* field_0x2e0;
};

struct UnkDemolitionLifeUpdate {
    u8 pad_0x0[0x360];
    UnkDemolitionLifeTarget* target;  // 0x360
    CVector4D current;                // 0x368, the start of a colour lerp (fn_71008c6b0c)
    CVector4D start;                  // 0x378
    CVector4D goal;                   // 0x388
};

static inline bool Near(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

// The same test with the two bounds compared in the other order (as the original does for w).
static inline bool NearW(f32 a, f32 b) {
    return a + 1e-5f >= b && a - 1e-5f <= b;
}

void CDemolitionBlockLifeComponent_UpdateLerpingNodeValue(UnkDemolitionLifeUpdate* self, f32 dt) {
    fn_7101160078(reinterpret_cast<UnkCommanderLife_A5_b63*>(self), dt);
    if (Near(self->current.x, self->goal.x) && Near(self->current.y, self->goal.y) &&
        Near(self->current.z, self->goal.z) && NearW(self->current.w, self->goal.w))
        return;
    if (!self->target)
        return;
    fn_71008c6b0c(reinterpret_cast<UnkColorLerp_A4_b75*>(&self->current), dt);
    UnkDemolitionLifeTarget* target = self->target;
    UnkVector4D colour(*static_cast<UnkVector4D*>(&self->current));
    Assign(target->colour, colour);
    if (target->field_0x2e0)
        *target->field_0x2e0->colour = colour;
}
