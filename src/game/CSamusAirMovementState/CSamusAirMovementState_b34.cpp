#include "game/CSamusMovementState/CParams.h"
#include "types.h"

struct UnkVec3Zero {
    f32 x, y, z;
};

struct CSamusAirMovementState {
    u8 unk_0x0[0x190];
    u64 field_0x190;
    u64 field_0x198;
    bool field_0x1a0;
    UnkVec3Zero field_0x1a4;
    u8 unk_0x1b0[0x210 - 0x1b0];
    f32 field_0x210;
    f32 field_0x214;
    bool field_0x218;
    bool field_0x219;
};

// Runs the base state's step, then clears the air state's fields.
void fn_710098923c(CSamusAirMovementState* self, void* arg) {
    fn_71009b0ce4(self, arg);
    self->field_0x1a4 = UnkVec3Zero{0, 0, 0};
    self->field_0x1a0 = false;
    self->field_0x210 = 0;
    self->field_0x214 = 0;
    self->field_0x218 = false;
    self->field_0x219 = false;
    self->field_0x190 = 0;
    self->field_0x198 = 0;
}
