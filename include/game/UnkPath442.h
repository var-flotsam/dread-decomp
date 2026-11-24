#pragma once

// A path of `count` points (tools/tu_map.py unit 442, src/game/UnkUnit442/UnkUnit442_SegmentValue.cpp): an open
// path (`closed` clear) has one segment fewer than points; each segment has a value (+0x48) and an entry (+0x70).
// Names and layout are placeholders.
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

    u32 SegmentCount() const { return count - ((closed == 0) & (count != 0)); }
};

// The segment's value, -1 past the end.
f32 fn_710035e544(const UnkPath442* self, u32 index);
// The address of the segment's entry, or of a shared fallback entry past the end.
const u32* GetPathSegmentEntryOrFallback(const UnkPath442* self, u32 index);
