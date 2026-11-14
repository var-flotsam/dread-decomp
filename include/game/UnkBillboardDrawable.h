#pragma once

// The drawable of the billboard (src/base/spatial/UnkBillboard.cpp, at +0x80; constructor fn_710019a4e8,
// unit 221 of tools/tu_map.py): a table of entries, each a run of 3x4 matrices and the matrices they were
// copied from. fn_710019b184 sets one of them. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMatrix44;

struct UnkBillboardDrawableEntry {
    f32* matrices;  // 0x0: 12 floats per slot
    u8 pad_0x8[8];
    u32 count;  // 0x10
    const UnkMatrix44** sources;  // 0x18
    u8 pad_0x20[8];
};
static_assert(sizeof(UnkBillboardDrawableEntry) == 0x28, "UnkBillboardDrawableEntry placeholder layout");

struct UnkBillboardDrawable {
    u8 pad_0x0[0x14];
    u32 count;  // 0x14
    UnkBillboardDrawableEntry* entries;  // 0x18
    UnkVec3f offset;  // 0x20
    u8 pad_0x2c[0xf8];
    u32 flags;  // 0x124: bit 2, move each matrix by the offset
};
static_assert(sizeof(UnkBillboardDrawable) == 0x128, "UnkBillboardDrawable placeholder layout");
