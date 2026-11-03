// CSpawnGroupComponent (tools/tu_map.py; the name from reflection): whether another spawn is allowed.
// A forced flag allows it; otherwise two counters must stay below their limits (a limit of 0 or less
// means none). Field names and layout are placeholders.
#include "types.h"

struct UnkSpawnGroup_b48a3 {
    u8 pad_0x0[0x13b];
    bool field_0x13b;
    u8 pad_0x13c[0x18c - 0x13c];
    s32 field_0x18c;
    s32 field_0x190;
    s32 field_0x194;
    s32 field_0x198;
};

bool fn_710111f610(UnkSpawnGroup_b48a3* self) {
    if (self->field_0x13b)
        return true;
    if (self->field_0x18c > 0 && self->field_0x194 >= self->field_0x18c)
        return false;
    if (self->field_0x190 > 0)
        return self->field_0x198 < self->field_0x190;
    return true;
}
