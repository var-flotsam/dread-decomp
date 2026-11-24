#pragma once

// One object that 2 files viewed in part (fn_710035e5f8 is declared with it in both; both views are 0x50 bytes, the members both name agree). Names and layouts are placeholders.
#include "types.h"
#include "base/math/UnkSegment2D.h"

struct UnkAIPathVertex;

struct UnkAIPath {
    UnkAIPathVertex* vertices;
    s32 count;
    u8 pad_0xc[0x20];
    u8 closed;
    u8 pad_0x2d[0x1b];
    f32* lengths;

    u32 EdgeCount() const;
    u32 NextEdge(u32 i) const;
    u32 PrevEdge(u32 i) const;
    f32 EdgeLength(u32 i) const;
    void Edge(UnkSegment2D* seg, u32 i) const;
};
