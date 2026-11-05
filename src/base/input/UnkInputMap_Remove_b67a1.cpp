// From unit 46 of tools/tu_map.py (the controllers' part of the input system): removes the entry
// stored under an id from a flat map (0x28-byte values at +0x18, u32 ids at +0x20, count at +0x28),
// id 0xfd meaning the current one at +8 (fn_710006d4d0). Names and layouts are placeholders.
#include "types.h"

struct UnkInputEntry_b67a1 {
    u8 data[0x28];
};

struct UnkInputEntryMap_b67a1 {
    UnkInputEntry_b67a1* values;
    u32* keys;
    u32 count;

    UnkInputEntry_b67a1* Find(u32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

void fn_710006d9bc(UnkInputEntryMap_b67a1* map, UnkInputEntry_b67a1* it);

struct UnkInputOwner_b67a1 {
    u8 pad_0x0[0x8];
    u32 current;  // 0x8
    u8 pad_0xc[0xc];
    UnkInputEntryMap_b67a1 map;  // 0x18
};

void fn_710006d4d0(UnkInputOwner_b67a1* self, u32 id) {
    if (id == 0xfd)
        id = self->current;
    UnkInputEntry_b67a1* it = self->map.Find(id);
    if (it != self->map.values + self->map.count)
        fn_710006d9bc(&self->map, it);
}
