// Passes the request on to fn_7100424b50 when the object is above its target's position by more
// than the component's offset (at 0x10c of the setup at 0x50) and horizontally further away than its
// limit (at 0x108); otherwise answers false.
#include "types.h"

struct UnkPositioned {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
};

struct UnkSetup {
    u8 pad_0x0[0x108];
    f32 field_0x108;
    f32 field_0x10c;
};

struct UnkRequester {
    u8 pad_0x0[8];
    UnkPositioned* field_0x8;
    u8 pad_0x10[0x40];
    UnkSetup* field_0x50;
};

UnkPositioned* fn_7100d7c990(UnkPositioned* object);
bool fn_7100424b50(UnkRequester* self, void* request, u32 flag);

bool fn_7100443b84(UnkRequester* self, void* request, u32 flag) {
    UnkPositioned* me = self->field_0x8;
    f32 x = me->field_0x44;
    f32 y = me->field_0x54;
    UnkPositioned* target = fn_7100d7c990(self->field_0x8);
    UnkSetup* setup = self->field_0x50;
    if (y > target->field_0x54 + setup->field_0x10c && __builtin_fabsf(x - target->field_0x44) > setup->field_0x108)
        return fn_7100424b50(self, request, flag);
    return false;
}
