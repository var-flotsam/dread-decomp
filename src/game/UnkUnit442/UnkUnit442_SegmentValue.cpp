// tools/tu_map.py unit 442 (beside src/game/CAIComponent.cpp): the per-segment value `index` (+0x48)
// of a path of `count` points (+0x8); an open path (+0x2c clear) has one segment fewer than points.
// -1 past the end. A second accessor hands out the address of the segment's entry (+0x70), or of a
// shared fallback entry. Names are placeholders.
#include "game/UnkPath442.h"
#include "types.h"

extern u32 g_unk_71726a4708;

f32 fn_710035e544(const UnkPath442* self, u32 index) {
    if (index < self->SegmentCount()) {
        return self->values[index];
    }
    return -1.0f;
}

const u32* fn_710035e580(const UnkPath442* self, u32 index) {
    if (index < self->SegmentCount()) {
        return &self->entries[index];
    }
    return &g_unk_71726a4708;
}
