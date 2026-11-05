// CHydrogigaZiplineRailComponent's unit: stores the "dead" flag and, when it changed, refreshes the
// rail (fn_710091d4bc, src/game/CHydrogigaZiplineRailComponent/CHydrogigaZiplineRailComponent_b49.cpp).
// Names are placeholders.
#include "types.h"

struct UnkZiplineRail_b49;
void fn_710091d4bc(UnkZiplineRail_b49* self);

struct UnkZiplineRail_A5_b65 {
    u8 pad_0x0[0x3d];
    bool dead;
};

void fn_710091d5c0(UnkZiplineRail_A5_b65* self, bool dead) {
    bool before = self->dead;
    self->dead = dead;
    if (before == dead)
        return;
    fn_710091d4bc(reinterpret_cast<UnkZiplineRail_b49*>(self));
}
