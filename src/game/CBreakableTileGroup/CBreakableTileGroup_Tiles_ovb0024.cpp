// CBreakableTileGroup's unit (tools/tu_map.py; the group of 0xa8-byte tiles, see
// src/game/CBreakableTileGroup.cpp): a tile's render object refreshed (fn_7100c9e968) or recreated
// from its type's model (fn_7100c9ea2c), a tile's four edges (fn_7100ca2ec4), resetting the tiles
// (fn_7100ca3518) and switching the group on or off (fn_7100ca0b44). Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU315.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkTileGroup;

// One tile of a group.
struct UnkQueryObj {
    u32 field_0x0;  // index
    u32 field_0x4;  // type
    u32 field_0x8;  // cell x
    u32 field_0xc;  // cell y
    u8 pad_0x10[0x18];
    f32 field_0x28;  // x
    f32 field_0x2c;  // y
    UnkTileGroup* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0xb];
    u64 field_0x48;
    u8 pad_0x50[0x58];
};
static_assert(sizeof(UnkQueryObj) == 0xa8, "UnkQueryObj placeholder layout");

struct UnkTileCollider;

// The group: its origin (+0x18), its size in cells (+0x24, +0x28), its tiles (+0x40, +0x48), the
// tile index of each cell (+0x58), a render parameter (+0xc8) and its collider (+0xd0).
struct UnkTileGroup {
    u8 pad_0x0[0x14];
    bool field_0x14;
    u8 pad_0x15[0x3];
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    u8 pad_0x2c[0x14];
    u32 field_0x40;
    UnkQueryObj* field_0x48;
    u8 pad_0x50[0x8];
    u32* field_0x58;
    u8 pad_0x60[0x68];
    u32 field_0xc8;
    UnkTileCollider* field_0xd0;
};
static_assert(__builtin_offsetof(UnkTileGroup, field_0xd0) == 0xd0, "UnkTileGroup placeholder layout");

static const u64 kNoRenderObject = 0xffffffff;

extern const UnkVec3f g_unk_710164ba64 __attribute__((visibility("hidden")));
UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale);
void fn_7100ca0c40(UnkTileGroup* self, u32 tile, UnkVec3f* pos);
void fn_7100ca0c78(UnkTileGroup* self, u32 tile, UnkVec3f* pos, UnkVec3f* scale);
void fn_7100ca1180(UnkTileGroup* group);
void fn_7100c9fdd8(UnkQueryObj* tile);
void fn_7100c9f9bc(UnkQueryObj* tile);
void fn_7100c9fc88(UnkQueryObj* tile);
struct UnkStruct_7100861148_Collider;
struct UnkTracker_71008618cc;
struct UnkReflObject;
void fn_7100d59994(UnkTracker_71008618cc* self, UnkReflObject* collider);

// Moves the tile's render object (when it has one) to the tile's place and scale.
void fn_7100c9e968(UnkQueryObj* self) {
    if (self->field_0x48 == kNoRenderObject)
        return;
    UnkVec3f pos(0.0f, 0.0f, 0.0f);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    fn_7100ca0c40(self->field_0x30, self->field_0x0, &pos);
    fn_7100ca0c78(self->field_0x30, self->field_0x0, &pos, &scale);
    UnkMatrix44 m = fn_71000791a8(pos, g_unk_710164ba64, scale);
    fn_7100219e64(g_unk_7101d47300, self->field_0x48, &m);
}

// Resets every tile (fn_7100c9fdd8), or only those without the +0x3c flag, and rebuilds the outlines
// when any was reset; whether that happened.
bool fn_7100ca3518(UnkTileGroup* self, bool all) {
    if ((s32)self->field_0x40 < 1)
        return false;
    if (all) {
        for (s64 i = 0; i < (s32)self->field_0x40; ++i)
            fn_7100c9fdd8(&self->field_0x48[i]);
    } else {
        bool any = false;
        for (s64 i = 0; i < (s32)self->field_0x40; ++i) {
            UnkQueryObj* tile = &self->field_0x48[i];
            if (!tile->field_0x3c) {
                fn_7100c9fdd8(tile);
                any = true;
            }
        }
        if (!any)
            return false;
    }
    fn_7100ca1180(self);
    return true;
}

// An edge of a tile: two points and a word set to -1.
struct UnkTileEdge {
    f32 x0, y0, x1, y1;
    f32 field_0x10;

    UnkTileEdge() : x0(0.0f), y0(0.0f), x1(0.0f), y1(0.0f), field_0x10(-1.0f) {}
    UnkTileEdge(f32 x0_, f32 y0_, f32 x1_, f32 y1_) : x0(x0_), y0(y0_), x1(x1_), y1(y1_), field_0x10(-1.0f) {}
};

struct UnkTileEdges {
    u32 count;
    UnkTileEdge data[4];

    UnkTileEdge& At(u32 i) {
        if (count <= i) {
            data[i] = UnkTileEdge();
            count = i + 1;
        }
        return data[i];
    }
};

// The tile's four edges, clockwise from the top one; an edge past the list's count is cleared first.
void fn_7100ca2ec4(UnkQueryObj* self, UnkTileEdges* out) {
    f32 x = self->field_0x28;
    f32 y = self->field_0x2c;
    f32 max_x = x + 100.0f;
    f32 max_y = y + 100.0f;
    out->At(0) = UnkTileEdge(x, max_y, max_x, max_y);
    out->At(1) = UnkTileEdge(max_x, max_y, max_x, y);
    out->At(2) = UnkTileEdge(max_x, y, x, y);
    out->At(3) = UnkTileEdge(x, y, x, max_y);
}

// The game manager's +0x1af8 as fn_7100c9ea2c reads it: a model per tile type at +0xc0.
struct UnkTileModels {
    u8 pad_0x0[0xc0];
    u64* field_0xc0;
};

// Drops the tile's render object and creates a new one from its type's model, when there is one.
void fn_7100c9ea2c(UnkQueryObj* self) {
    if (self->field_0x48 != kNoRenderObject) {
        fn_7100219e10(g_unk_7101d47300, self->field_0x48);
        self->field_0x48 = kNoRenderObject;
    }
    u32 type = self->field_0x4;
    UnkTileModels* models = reinterpret_cast<UnkTileModels*>(g_unk_7101d47310->field_0x1af8);
    u64 model = models->field_0xc0[type];
    if (!model)
        return;
    UnkVec3f pos(0.0f, 0.0f, 0.0f);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    fn_7100ca0c40(self->field_0x30, self->field_0x0, &pos);
    UnkMatrix44 m = fn_71000791a8(pos, g_unk_710164ba64, scale);
    fn_7100219dd0(g_unk_7101d47300, model, &m, 1, 1, &self->field_0x48, self->field_0x30->field_0xc8);
}

// Switches the group on or off: its collider is added to or removed from the collision tracker and
// each tile is told (fn_7100c9f9bc on, fn_7100c9fc88 off).
void fn_7100ca0b44(UnkTileGroup* self, bool enable) {
    if (self->field_0x14 == enable)
        return;
    self->field_0x14 = enable;
    if (enable) {
        if (self->field_0xd0 && g_unk_7101d47310->field_0x608)
            fn_7100d59994(static_cast<UnkTracker_71008618cc*>(g_unk_7101d47310->field_0x608->field_0x820),
                          reinterpret_cast<UnkReflObject*>(self->field_0xd0));
        for (s64 i = 0; i < (s32)self->field_0x40; ++i)
            fn_7100c9f9bc(&self->field_0x48[i]);
    } else {
        for (s64 i = 0; i < (s32)self->field_0x40; ++i)
            fn_7100c9fc88(&self->field_0x48[i]);
        if (self->field_0xd0 && g_unk_7101d47310->field_0x608)
            fn_7100d5abfc(g_unk_7101d47310->field_0x608->field_0x820,
                          reinterpret_cast<UnkStruct_7100861148_Collider*>(self->field_0xd0));
    }
}
