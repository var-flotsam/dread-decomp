// CBreakableTileGroupSonarTargetComponent unit (tools/tu_map.py unit 1517): collects the intact tiles of
// the target tile group whose centre lies within `range` of a point: each one's marker is taken from the
// markers already shown, or else made from the pool, and added to the output list (at most 64).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CBreakableTileGroup.h"
#include "game/operator_new.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;

struct UnkTile_la0e310 {
    u8 pad_0x0[0x28];
    f32 x;  // 0x28
    f32 y;  // 0x2c
    u8 pad_0x30[0x8];
    s32 field_0x38;  // nonzero once broken
};

struct UnkSonarTileGroup_la0e310 {
    u8 pad_0x0[0x40];
    u32 count;  // 0x40
};

struct UnkSonarTarget_la0e310 {
    u8 pad_0x0[0xa0];
    UnkSonarTileGroup_la0e310* group;  // 0xa0
};

struct UnkBreakableTileGroupSonarTarget_la0e310 {
    u8 pad_0x0[0x40];
    UnkWeakHandle* handle;  // 0x40
};

// A marker shown on a tile.
struct UnkTileMarker_la0e310 {
    u8 pad_0x0[0x8];
    UnkTile_la0e310* tile;  // 0x8
};

// A fixed list of up to 64 markers, kept in raw storage after its count.
struct UnkMarkerList_la0e310 {
    UnkTileMarker_la0e310** Data() { return reinterpret_cast<UnkTileMarker_la0e310**>(storage); }

    // Appends `marker` and returns the new count.
    u32 PushBack(UnkTileMarker_la0e310* marker) {
        u32 n = count;
        Data()[n] = marker;
        count = n + 1;
        return n + 1;
    }

    u32 count;
    u8 storage[64 * sizeof(UnkTileMarker_la0e310*)];
};

// The pool markers are made from.
struct UnkMarkerPool_la0e310 {
    u8 pad_0x0[0x10];
    u8 allocator[0x50];  // 0x10
    u64 free;  // 0x60
};

// A callback bound to an object.
struct UnkCallback_la0e310 {
    UnkBreakableTileGroupSonarTarget_la0e310* object;
    void (*function)(char*, char*);
};

extern UnkMarkerPool_la0e310 g_unk_71727436c0;
extern void* g_unk_71727436b8;
void fn_7100a0e69c(char* p0, char* p1);
void fn_7100a0e71c(char* p0, char* p1);
void fn_7100a0ea10(UnkTileMarker_la0e310* self, UnkTile_la0e310* tile, void* owner, UnkCallback_la0e310* on_a,
                   UnkCallback_la0e310* on_b);

static inline UnkTileMarker_la0e310* FindMarker(UnkMarkerList_la0e310* list, UnkTile_la0e310* tile) {
    UnkTileMarker_la0e310* found = nullptr;
    for (UnkTileMarker_la0e310** it = list->Data(); it != list->Data() + list->count; ++it) {
        if (*it && (*it)->tile == tile)
            found = *it;
    }
    return found;
}

void fn_7100a0e310(UnkBreakableTileGroupSonarTarget_la0e310* self, const CVector2D* point, UnkMarkerList_la0e310* shown,
                   UnkMarkerList_la0e310* out, f32 range) {
    UnkSonarTileGroup_la0e310* group =
        reinterpret_cast<UnkSonarTarget_la0e310*>(self->handle->target)->group;
    f32 range_sq = range * range;
    for (u32 i = 0; i < group->count; i++) {
        UnkTile_la0e310* tile = reinterpret_cast<UnkTile_la0e310*>(fn_7100ca1ea8(group, i));
        if (!tile || tile->field_0x38 != 0)
            continue;
        f32 dx = point->x - (tile->x + 50.0f);
        f32 dy = point->y - (tile->y + 50.0f);
        if (dx * dx + dy * dy > range_sq)
            continue;
        UnkTileMarker_la0e310* marker = FindMarker(shown, tile);
        if (!marker) {
            if (!g_unk_71727436c0.free)
                continue;
            g_unk_71727436c0.free--;
            marker = static_cast<UnkTileMarker_la0e310*>(
                fn_7100083804(reinterpret_cast<UnkVoicePool_b56*>(g_unk_71727436c0.allocator)));
            UnkCallback_la0e310 on_a = {self, fn_7100a0e69c};
            UnkCallback_la0e310 on_b = {self, fn_7100a0e71c};
            fn_7100a0ea10(marker, tile, g_unk_71727436b8, &on_a, &on_b);
            if (!marker)
                continue;
        }
        if (out->PushBack(marker) == 64)
            return;
    }
}
