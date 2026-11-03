// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py unit 2515): whether any
// 0x48-byte entry of the array (data, count) answers fn_7100116de0 for `query` (with 0, 1 and a
// scale of 1). Names and layouts are placeholders.
#include "types.h"

struct UnkEntry;  // src/game_s2/fn_71005528fc.cpp
bool fn_7100116de0(UnkEntry* entry, void* query, u32 a, u32 b, f32 scale);

struct UnkPolygonEntry_b46a3 {
    u8 data[0x48];
};

struct UnkPolygonEntries_b46a3 {
    UnkPolygonEntry_b46a3* data;
    u32 count;
};

bool fn_7100d65fb0(const UnkPolygonEntries_b46a3* self, void* query) {
    for (UnkPolygonEntry_b46a3* e = self->data, *end = self->data + self->count; e != end; ++e)
        if (fn_7100116de0(reinterpret_cast<UnkEntry*>(e), query, 0, 1, 1.0f))
            return true;
    return false;
}
