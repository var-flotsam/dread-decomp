// Forwards to fn_7100ac4ec0 (after setting +0x5d8) when the argument is the object behind +0x30 and +0x5e4 is clear.
#include "game/UnkActor.h"
#include "types.h"

struct UnkObj {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x5a0];
    u32 field_0x5d8;
    u8 pad_0x5dc[8];
    u8 field_0x5e4;
};

bool fn_7100ac4ec0(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5);

bool fn_7100503068(UnkObj* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    if (fn_7100d7c990(self->field_0x30) != a1 || self->field_0x5e4)
        return false;
    self->field_0x5d8 = 1;
    return fn_7100ac4ec0(self, a1, a2, a3, a4, a5);
}
