// CSpawnGroupComponent (tools/tu_map.py; the name from reflection): whether another spawn is allowed.
// A forced flag allows it; otherwise two counters must stay below their limits (a limit of 0 or less
// means none). Field names and layout are placeholders.
#include "types.h"

struct UnkSpawnGroup_b48a3 {
    u8 pad_0x0[0x13b];
    bool bIsInfinite;
    u8 pad_0x13c[0x18c - 0x13c];
    s32 iMaxToGenerate;
    s32 iMaxSimultaneous;
    s32 field_0x194;
    s32 field_0x198;
};

bool fn_710111f610(UnkSpawnGroup_b48a3* self) {
    if (self->bIsInfinite)
        return true;
    if (self->iMaxToGenerate > 0 && self->field_0x194 >= self->iMaxToGenerate)
        return false;
    if (self->iMaxSimultaneous > 0)
        return self->field_0x198 < self->iMaxSimultaneous;
    return true;
}
