// UnkTU367: whether a navigation region counts as blocked for the given flags: by its own flag, by the
// state of its tile's query object, or (flag bit 4) by a locked door among the tile's links.
// Names and layouts are placeholders.
#include "game/CBreakableTileGroup.h"
#include "game/UnkTU367.h"
#include "types.h"

UnkQueryObj* fn_71002b37d8(void* tile);
bool fn_71008d0db4(UnkDoorLock* door);

struct UnkLinkTarget_l2af6fc {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

struct UnkLink_l2af6fc {
    UnkLinkTarget_l2af6fc* link;  // 0x0
    u8 pad_0x8[0x10];
};

struct UnkTile_l2af6fc {
    u8 pad_0x0[0x124];
    u32 link_count;                // 0x124
    u8 pad_0x128[0x8];
    UnkLink_l2af6fc links[1];      // 0x130
};

struct UnkRegionView_l2af6fc {
    u8 pad_0x0[0x60];
    bool blocked;           // 0x60
    u8 pad_0x61[0x7];
    UnkTile_l2af6fc* tile;  // 0x68
};

static_assert(__builtin_offsetof(UnkTile_l2af6fc, links) == 0x130, "");

// Whether the tile's query object is in `state`; false without one. `already` short-cuts the state test.
static inline bool TileInState(UnkRegionView_l2af6fc* region, bool already, u32 state) {
    if (region->tile == nullptr)
        return false;
    UnkQueryObj* query = fn_71002b37d8(region->tile);
    if (query == nullptr)
        return false;
    return already | (fn_7100c9e960(query) == state);
}

bool fn_71002af6fc(UnkRegion_710027a41c* region_, u32 flags) {
    UnkRegionView_l2af6fc* region = reinterpret_cast<UnkRegionView_l2af6fc*>(region_);
    if (region->blocked)
        return true;

    bool result = flags & 1;
    if ((flags & 3) == 2)
        result = TileInState(region, result, 1);
    if ((flags & 4) && !result)
        result = TileInState(region, result, 7);
    if ((flags & 8) && !result)
        result = TileInState(region, result, 2);
    if ((flags & 0x10) && !result) {
        if (UnkTile_l2af6fc* tile = region->tile) {
            for (UnkLink_l2af6fc* it = tile->links; it != tile->links + tile->link_count; ++it) {
                if (it->link == nullptr || it->link->target == nullptr)
                    continue;
                UnkDoorLock* door = fn_71002af860(it->link->target);
                if (door != nullptr && fn_71008d0db4(door))
                    return true;
            }
        }
    }
    return result || region->blocked;
}
