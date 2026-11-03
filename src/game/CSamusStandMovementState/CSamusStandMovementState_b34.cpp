#include "types.h"

struct UnkStandParams {
    u8 unk_0x0[0xc8];
    f32 field_0xc8;
};

struct CSamusStandMovementState {
    u8 unk_0x0[0x40];
    UnkStandParams* params;
    u8 unk_0x48[0x190 - 0x48];
    f32 field_0x190;
};

// In mode 1, whether the timer at 0x190 is within [0, params->field_0xc8).
bool fn_71009b49a8(CSamusStandMovementState* self, s32 mode) {
    if (mode == 1 && self->field_0x190 >= 0.0f && self->field_0x190 < self->params->field_0xc8)
        return true;
    return false;
}
