// Unit 46 of tools/tu_map.py (the input system's controller part): whether the binding stored under a controller id
// (0xfd meaning the current one at +8) holds an item; the flat map is the one of
// src/base/input/UnkInputMap_Remove_b67a1.cpp. Names and layouts are placeholders.
#include "types.h"

struct UnkInputEntry_b0232 {
    u32* items;  // 0x0
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];
};

struct UnkInputEntryMap_b0232 {
    UnkInputEntry_b0232* values;  // 0x0
    u32* keys;  // 0x8
    u32 count;  // 0x10

    UnkInputEntry_b0232* Find(u32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkInputOwner_b0232 {
    u8 pad_0x0[0x8];
    u32 current;  // 0x8
    u8 pad_0xc[0xc];
    UnkInputEntryMap_b0232 map;  // 0x18
};

bool fn_710006d7d8(UnkInputOwner_b0232* self, u32 item, u32 id) {
    if (id == 0xfd)
        id = self->current;
    UnkInputEntry_b0232* it = self->map.Find(id);
    if (it == self->map.values + self->map.count)
        return false;
    u32* p = it->items;
    for (u64 n = it->count; n != 0; --n, ++p)
        if (*p == item)
            return true;
    return false;
}
