// CTunableAIManager's unit: looks up the entry keyed by a u32 id in the flat map at +0xbc8 and, if it holds
// any 0x18-byte items, hands the last one's vector (+8) to fn_710007b444 on the given object. Names and layouts
// are placeholders.
#include "game/UnkTU55.h"
#include "types.h"

struct UnkTuneItem_ldaad48 {
    u8 pad_0x0[0x8];
    u8 field_0x8[0x10];
};

struct UnkTuneEntry_ldaad48 {
    UnkTuneItem_ldaad48* items;
    u32 count;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkTuneMgr_ldaad48 {
    u8 pad_0x0[0xbc8];
    UnkTuneEntry_ldaad48* values;
    u32* keys;
    u32 count;

    UnkTuneEntry_ldaad48* Find(u32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
    UnkTuneEntry_ldaad48& At(u32 key) {
        u64 i = 0;
        while (i < count && keys[i] != key)
            ++i;
        return values[i];
    }
};

bool fn_7100daad48(UnkTuneMgr_ldaad48* self, u32 id, void* target) {
    if (self->Find(id) == self->values + self->count)
        return false;
    UnkTuneEntry_ldaad48& entry = self->At(id);
    if (entry.count == 0)
        return false;
    fn_710007b444(target, reinterpret_cast<const UnkVec3*>(entry.items[u64(entry.count) - 1].field_0x8));
    return true;
}
