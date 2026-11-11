// CBreakableTileGroup's unit (tools/tu_map.py; see src/game/CBreakableTileGroup.cpp): the tile
// arrays made for a capacity (fn_7100ca0dec), a tile added at a position snapped to the 100-unit grid
// (fn_7100ca0cb0), three lists cleared and freed (fn_7100c9da0c) and the tiles of a kind set off
// with a delay that grows with their distance from a tile (fn_7100ca35ec). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/CBreakableTileGroup.h"
#include "types.h"

struct UnkQueryObj {
    u32 field_0x0;  // index
    u32 field_0x4;  // type
    u32 field_0x8;  // cell x
    u32 field_0xc;  // cell y
    u8 pad_0x10[0x18];
    f32 field_0x28;  // x
    f32 field_0x2c;  // y
    void* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0x6b];
};
static_assert(sizeof(UnkQueryObj) == 0xa8, "UnkQueryObj placeholder layout");

// A tile as the array's constructor and destructor see it.
struct UnkBreakableTile_b49 {
    u8 data[0xa8];
    UnkBreakableTile_b49() asm("fn_7100c9db18");
    ~UnkBreakableTile_b49() asm("fn_7100c9dd20");
};

struct UnkTilePos {
    f32 x, y;
};

struct UnkTileGroupB {
    u8 pad_0x0[0x20];
    f32 field_0x20;
    u8 pad_0x24[0x8];
    f32 field_0x2c;  // min x
    f32 field_0x30;  // min y
    f32 field_0x34;  // max x
    f32 field_0x38;  // max y
    u32 field_0x3c;  // capacity
    u32 field_0x40;  // count
    UnkBreakableTile_b49* field_0x48;
    UnkTilePos* field_0x50;
    u32* field_0x58;
};


// Recreates the tile array (with its count in front) and the position array for `capacity` tiles,
// frees the cell array and empties the group.
void fn_7100ca0dec(UnkTileGroupB* self, u32 capacity) {
    self->field_0x3c = capacity;
    if (self->field_0x48) {
        delete[] self->field_0x48;
        self->field_0x48 = nullptr;
    }
    u32 n = self->field_0x3c;
    u64* block = static_cast<u64*>(fn_7100083ba0((u64)n * sizeof(UnkBreakableTile_b49) + 8, 5));
    *block = n;
    self->field_0x48 = new (block + 1) UnkBreakableTile_b49[n];
    if (self->field_0x50) {
        delete[] self->field_0x50;
        self->field_0x50 = nullptr;
    }
    u32 m = self->field_0x3c;
    UnkTilePos* pos = static_cast<UnkTilePos*>(fn_7100083ba0((u64)m * sizeof(UnkTilePos), 5));
    if (m)
        __builtin_memset(pos, 0, (u64)m * sizeof(UnkTilePos));
    self->field_0x50 = pos;
    if (self->field_0x58) {
        delete[] self->field_0x58;
        self->field_0x58 = nullptr;
    }
    self->field_0x40 = 0;
}

void fn_7100c9f8fc(UnkQueryObj* tile);

// Adds a tile at `pos` (snapped to the grid), growing the group's bounds; null when it is full.
UnkQueryObj* fn_7100ca0cb0(UnkTileGroupB* self, void* unused, const f32* pos) {
    u32 n = self->field_0x40;
    if (n >= self->field_0x3c)
        return nullptr;
    f32 x = __builtin_ceilf((pos[0] - 50.0f) / 100.0f) * 100.0f - 50.0f;
    f32 y = __builtin_ceilf((pos[1] - 50.0f) / 100.0f) * 100.0f - 50.0f;
    if (n == 0) {
        self->field_0x20 = pos[2];
        self->field_0x2c = x;
        self->field_0x30 = y;
        self->field_0x34 = x;
        self->field_0x38 = y;
    } else {
        if (pos[0] < self->field_0x2c)
            self->field_0x2c = x;
        if (pos[1] < self->field_0x30)
            self->field_0x30 = y;
        if (pos[0] > self->field_0x34)
            self->field_0x34 = x;
        if (pos[1] > self->field_0x38)
            self->field_0x38 = y;
    }
    UnkQueryObj* tile = reinterpret_cast<UnkQueryObj*>(&self->field_0x48[n]);
    tile->field_0x0 = n;
    fn_7100c9f8fc(tile);
    tile->field_0x30 = self;
    self->field_0x50[self->field_0x40] = UnkTilePos{x, y};
    self->field_0x40++;
    return tile;
}

// Three vectors; the first holds pointers to words that are cleared before it is freed.
struct UnkTileLists {
    base::UnkVector<u64*> field_0x0;
    base::UnkVector<u64> field_0x28;
    base::UnkVector<u64> field_0x50;
};

void fn_7100c9da0c(UnkTileLists* self) {
    for (u64** it = self->field_0x0.data, **end = it + self->field_0x0.count; it != end; ++it)
        **it = 0;
    self->field_0x0.count = 0;
    self->field_0x28.count = 0;
    self->field_0x50.count = 0;
    self->field_0x50.Free();
    self->field_0x28.Free();
    self->field_0x0.Free();
}

struct UnkTunable;
extern UnkTunable g_unk_7172796920;
extern "C" f32 sqrtf(f32);
void fn_7100c9feac(UnkQueryObj* tile, s32 a, s32 b, f32 delay);

struct UnkTileGroupC {
    u8 pad_0x0[0x40];
    u32 field_0x40;
    UnkQueryObj* field_0x48;
};

// Sets off (fn_7100c9feac) each tile of `kind` that is not broken, after a delay of the tunable's
// +0x44 (halved when b is 1) per cell of distance from the tile `from`.
void fn_7100ca35ec(UnkTileGroupC* self, u32 from, u32 kind, s32 a, s32 b) {
    f32 speed = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_7172796920) + 0x44);
    if (b == 1)
        speed = speed * 0.5f;
    if (self->field_0x40 == 0)
        return;
    UnkQueryObj* origin = &self->field_0x48[from];
    f32 ox = (f32)origin->field_0x8;
    f32 oy = (f32)origin->field_0xc;
    for (u32 i = 0; i < self->field_0x40; i++) {
        UnkQueryObj* tile = self->field_0x48 + i;
        if (fn_7100c9e960(tile) != kind || tile->state == 2)
            continue;
        f32 dx = ox - (f32)tile->field_0x8;
        f32 dy = oy - (f32)tile->field_0xc;
        fn_7100c9feac(tile, a, b, speed * sqrtf(dx * dx + dy * dy));
    }
}
