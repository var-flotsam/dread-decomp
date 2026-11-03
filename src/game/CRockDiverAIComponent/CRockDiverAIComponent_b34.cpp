#include "types.h"

struct CRockDiverAIComponent {
    u8 unk_0x0[0x146dc];
    s32 phase;
};

// Advances the phase from 0 to 1 and from 1 to 2.
void fn_7100663d2c(CRockDiverAIComponent* self) {
    s32& phase = self->phase;
    if (phase == 0)
        phase = 1;
    else if (phase == 1)
        phase = 2;
}
