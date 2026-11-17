// UnkTU2506 (tools/tu_map.py unit 2506): links two ids both ways in a dictionary of flag masks: b's
// flags join a's mask and a's join b's, adding either id that has no mask yet. Names and layouts are
// placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "types.h"

using base::global::CRntSmallDictionary;

typedef CRntSmallDictionary<u32, u32> UnkMaskMap_ld49144;

struct UnkMaskOwner_ld49144 {
    u8 pad_0x0[0xa0];
    UnkMaskMap_ld49144 masks;  // 0xa0
};

// The index of the key, or the count when it is missing.
static inline u64 IndexOf(const UnkMaskMap_ld49144* map, u32 key) {
    for (u64 i = 0; i < map->count; ++i) {
        if (map->keys[i] == key)
            return i;
    }
    return map->count;
}

// Adds `flags` to the key's mask, or gives the key the mask `flags` when it has none.
static inline void AddFlags(UnkMaskMap_ld49144* map, u32 key, u32 flags) {
    if (map->Find(key) != map->End()) {
        u32 mask = map->values[IndexOf(map, key)] | flags;
        map->values[IndexOf(map, key)] = mask;
    } else {
        if (map->count == map->capacity)
            map->Grow(map->count + map->step);
        map->keys[map->count] = key;
        map->values[map->count] = flags;
        ++map->count;
    }
}

void fn_7100d49144(UnkMaskOwner_ld49144* self, u32 a, u32 b) {
    AddFlags(&self->masks, a, b);
    AddFlags(&self->masks, b, a);
}
