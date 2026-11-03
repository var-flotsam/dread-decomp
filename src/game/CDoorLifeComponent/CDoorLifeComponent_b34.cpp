#include "types.h"

struct UnkDoorLifeState {
    u8 unk_0x0[0x128];
    bool field_0x128;
};

struct CDoorLifeComponent {
    u8 unk_0x0[0x644];
    u32 field_0x644;
    u32 field_0x648;
    u8 unk_0x64c[0x9f8 - 0x64c];
    UnkDoorLifeState* state;
};

// Whether exactly one of the two counters at 0x644 and 0x648 is set (never while the state's flag is).
bool fn_71008d9b24(CDoorLifeComponent* self) {
    if (self->state->field_0x128)
        return false;
    if (self->field_0x644 != 0 && self->field_0x648 == 0)
        return true;
    if (self->field_0x644 == 0 && self->field_0x648 != 0)
        return true;
    return false;
}
