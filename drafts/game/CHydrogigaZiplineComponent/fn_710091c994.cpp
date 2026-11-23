// CHydrogigaZiplineComponent's unit (tools/tu_map.py unit 1410): records the rail part's flag and moves its
// state machine (+0x280, "none" 6) on: when set, from state 1 to 2 or from 5 to 3; when cleared, from 2 to 1
// or else to 5 (not from 1). Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "game/CHydrogigaZiplineComponent.h"
#include "types.h"

struct UnkZiplineRailPart_b49 {
    u8 pad_0x0[0x48];
    bool enabled;  // 0x48
    u8 pad_0x49[0x280 - 0x49];
    UnkSmMachine_A5_b73 machine;  // 0x280
    u8 pad_0x330[0x531 - 0x330];
    bool dead;  // 0x531
};
static_assert(__builtin_offsetof(UnkZiplineRailPart_b49, dead) == 0x531, "layout");

static inline void Request(UnkZiplineRailPart_b49* self, u32 id) {
    RequestState_A5_b73<15, 6>(&self->machine, id);
}

void fn_710091c994(UnkZiplineRailPart_b49* self, bool dead) {
    self->dead = dead;
    if (dead) {
        if (!self->enabled)
            return;
        UnkSmState_A5_b73* current = self->machine.current;
        if (!current)
            return;
        if (current->id == 5)
            Request(self, 3);
        else if (current->id == 1)
            Request(self, 2);
    } else {
        UnkSmState_A5_b73* current = self->machine.current;
        if (current && current->id == 1)
            return;
        if (current && current->id == 2)
            Request(self, 1);
        else
            Request(self, 5);
    }
}
