// CHydrogigaZiplineRailComponent's unit: stores the "dead" flag and, when it changed, refreshes the
// rail (fn_710091d4bc, src/game/CHydrogigaZiplineRailComponent/CHydrogigaZiplineRailComponent_b49.cpp).
// Names are placeholders.
#include "game/CHydrogigaZiplineRailComponent.h"
#include "types.h"

struct UnkZiplineRail_b49;

struct UnkZiplineRail_A5_b65 {
    u8 pad_0x0[0x3d];
    bool dead;
};

void CHydrogigaZiplineRailComponent_SetHydrogigaDead(UnkZiplineRail_A5_b65* self, bool dead) {
    bool before = self->dead;
    self->dead = dead;
    if (before == dead)
        return;
    CHydrogigaZiplineRailComponent_ApplyDeadState(reinterpret_cast<UnkZiplineRail_b49*>(self));
}
