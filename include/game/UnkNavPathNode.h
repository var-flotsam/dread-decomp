#pragma once

#include "game/UnkNavPathPoint.h"
#include "types.h"

struct UnkNavPathNode;

// The node: the payload, then the cell, side and kind, a word, the position, the three costs and the
// parent (0x128 bytes).
struct UnkNavPathNode {
    u8 data[0x100];
    u16 cell;
    u8 side;
    u32 kind;
    u32 field_0x108;
    UnkNavPathPoint pos;
    f32 costs[3];
    UnkNavPathNode* parent;
};
