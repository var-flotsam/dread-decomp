// UnkTU367: the crossing test of a navigation cell's edge: whether the cell on the other side can be
// entered for the given flags (its tile's query state, a locked door among the tile's links, its flag).
// Names and layouts are placeholders.
#include "game/CBreakableTileGroup.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU367.h"
#include "types.h"

UnkQueryObj* fn_71002b37d8(void* tile);
bool fn_71008d0db4(UnkDoorLock* door);

struct UnkLinkTarget_l2afcd8 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

struct UnkLink_l2afcd8 {
    UnkLinkTarget_l2afcd8* link;  // 0x0
    u8 pad_0x8[0x10];
};

struct UnkTile_l2afcd8 {
    u8 pad_0x0[0x124];
    u32 link_count;            // 0x124
    u8 pad_0x128[0x8];
    UnkLink_l2afcd8 links[1];  // 0x130
};
static_assert(__builtin_offsetof(UnkTile_l2afcd8, links) == 0x130, "");

// The members of a cell this function reads beyond game/UnkNavMeshCell.h's.
struct UnkCellView_l2afcd8 {
    u8 pad_0x0[0x58];
    UnkNavMesh* mesh;       // 0x58
    bool blocked;           // 0x60
    u8 pad_0x61[0x7];
    UnkTile_l2afcd8* tile;  // 0x68
};

// The members of an edge this function reads beyond game/UnkNavMeshCell.h's.
struct UnkEdgeView_l2afcd8 {
    u8 index;       // 0x0
    u8 pad_0x1;
    u16 neighbour;  // 0x2
    u8 field_0x4;   // 0x4
};

// The query object of the cell's tile, if it has both.
static inline UnkQueryObj* TileQuery(UnkCellView_l2afcd8* cell) {
    if (cell->tile == nullptr)
        return nullptr;
    return fn_71002b37d8(cell->tile);
}

bool fn_71002afcd8(UnkNavMeshCell* cell_, UnkNavMeshEdge* edge_, u32 flags) {
    UnkCellView_l2afcd8* cell = reinterpret_cast<UnkCellView_l2afcd8*>(cell_);
    UnkEdgeView_l2afcd8* edge = reinterpret_cast<UnkEdgeView_l2afcd8*>(edge_);
    UnkNavMesh* mesh = cell->mesh;
    if (mesh == nullptr)
        return false;
    if (edge->index == 0xff)
        return false;
    if (edge->neighbour == 0xffff || edge->field_0x4 == 0xff)
        return false;

    UnkCellView_l2afcd8* other =
        reinterpret_cast<UnkCellView_l2afcd8*>(fn_710027a33c(mesh, edge->neighbour));
    if (flags & 1)
        return true;

    bool open = true;
    if (flags & 2) {
        if (UnkQueryObj* query = TileQuery(other))
            open &= fn_7100c9e960(query) != 1;
    }
    if ((flags & 4) && open) {
        if (UnkQueryObj* query = TileQuery(other))
            open &= fn_7100c9e960(query) != 7;
    }
    if ((flags & 8) && open) {
        if (UnkQueryObj* query = TileQuery(other))
            open &= fn_7100c9e960(query) != 2;
    }
    if ((flags & 0x10) && open) {
        if (UnkTile_l2afcd8* tile = other->tile) {
            for (UnkLink_l2afcd8* it = tile->links; it != tile->links + tile->link_count; ++it) {
                if (it->link == nullptr || it->link->target == nullptr)
                    continue;
                UnkDoorLock* door = fn_71002af860(it->link->target);
                if (door != nullptr && fn_71008d0db4(door))
                    return true;
            }
        }
    }
    if (!open)
        return true;
    return other->blocked;
}
