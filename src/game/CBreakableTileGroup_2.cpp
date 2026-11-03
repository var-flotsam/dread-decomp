// The CBreakableTileGroup unit (see CBreakableTileGroup.cpp): a group of 0xa8-byte tiles in an
// array at +0x48, counted at +0x40. Names and layouts are placeholders.
#include "types.h"

struct UnkTileGroupTile {
    u8 unk_0x0[0xa8];
};

struct UnkTileGroupTiles {
    u8 unk_0x0[0x40];
    s32 count;
    UnkTileGroupTile* tiles;  // +0x48
};

// Advances one tile by `dt`.
void fn_7100c9dfd4(UnkTileGroupTile* tile, f32 dt);

// Advances every tile of the group by `dt`.
void fn_7100ca0a84(UnkTileGroupTiles* self, f32 dt) {
    for (s64 i = 0; i < self->count; ++i)
        fn_7100c9dfd4(&self->tiles[i], dt);
}
