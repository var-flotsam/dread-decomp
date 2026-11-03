// tools/tu_map.py unit 442 (beside src/game/CAIComponent.cpp): the per-segment value `index` (+0x48)
// of a path of `count` points (+0x8); an open path (+0x2c clear) has one segment fewer than points.
// -1 past the end. A second accessor hands out the address of the segment's entry (+0x70), or of a
// shared fallback entry. Names are placeholders.
#include "types.h"

struct UnkPath442 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    u8 closed;  // 0x2c
    u8 pad_0x2d[0x1b];
    f32* values;  // 0x48
    u8 pad_0x50[0x20];
    u32* entries;  // 0x70
};

extern u32 g_unk_71726a4708;

static inline u32 SegmentCount442(const UnkPath442* self) {
    return self->count - ((self->closed == 0) & (self->count != 0));
}

f32 fn_710035e544(const UnkPath442* self, u32 index) {
    if (index < SegmentCount442(self)) {
        return self->values[index];
    }
    return -1.0f;
}

const u32* fn_710035e580(const UnkPath442* self, u32 index) {
    if (index < SegmentCount442(self)) {
        return &self->entries[index];
    }
    return &g_unk_71726a4708;
}
