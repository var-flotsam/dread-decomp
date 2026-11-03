// A CBossSpawnGroupComponent check: fn_710111bb34 holds and fn_710111f07c returns the count at +0x70.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBossSpawnGroupComponent {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};

bool fn_710111bb34(UnkBossSpawnGroupComponent* self);
s32 fn_710111f07c(UnkBossSpawnGroupComponent* self);

bool fn_7100a10f88(UnkBossSpawnGroupComponent* self) {
    if (!fn_710111bb34(self))
        return false;
    return fn_710111f07c(self) == self->field_0x70;
}
