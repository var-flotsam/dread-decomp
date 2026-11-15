// CTunableAIManager's unit: whether the entry keyed by a u32 id in the flat map at +0xbc8 (0x28-byte values,
// u32 keys at +0xbd0, count at +0xbd8) is set (+8). Names and layouts are placeholders.
#include "types.h"

struct UnkTuneEntry_ldaaae0 {
    u64 field_0x0;
    u32 field_0x8;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkTuneMgr_ldaaae0 {
    u8 pad_0x0[0xbc8];
    UnkTuneEntry_ldaaae0* values;
    u32* keys;
    u32 count;

    UnkTuneEntry_ldaaae0* Find(u32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
    UnkTuneEntry_ldaaae0& At(u32 key) {
        u64 i;
        for (i = 0; i < count; ++i)
            if (keys[i] == key)
                break;
        return values[i];
    }
};

bool fn_7100daaae0(UnkTuneMgr_ldaaae0* self, u32 id) {
    UnkTuneEntry_ldaaae0* it = self->Find(id);
    if (it == self->values + self->count)
        return false;
    return self->At(id).field_0x8 != 0;
}
