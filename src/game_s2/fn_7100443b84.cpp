// Passes the request on to fn_7100424b50 when the object is above its target's position by more
// than the component's offset (at 0x10c of the setup at 0x50) and horizontally further away than its
// limit (at 0x108); otherwise answers false.
#include "game/UnkActor.h"
#include "types.h"

struct UnkSetup {
    u8 pad_0x0[0x108];
    f32 field_0x108;
    f32 field_0x10c;
};

struct UnkRequester {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x40];
    UnkSetup* field_0x50;
};

bool fn_7100424b50(UnkRequester* self, void* request, u32 flag);

bool fn_7100443b84(UnkRequester* self, void* request, u32 flag) {
    UnkActor* me = self->field_0x8;
    f32 x = me->field_0x44;
    f32 y = me->field_0x54;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    UnkSetup* setup = self->field_0x50;
    if (y > target->field_0x54 + setup->field_0x10c && __builtin_fabsf(x - target->field_0x44) > setup->field_0x108)
        return fn_7100424b50(self, request, flag);
    return false;
}
