// CWideBeamGun's unit (tools/tu_map.py): whether a hit lands on a breakable tile in state 1. The hit's
// object (+0x0, +0x10, +0x10) must be a CBreakableTileGroup; the tile at the hit's point (+0x14, with the
// normal at +0x20 when bit 5 of +0x3c is set) is looked up with fn_7100ca1ecc and its state read with
// fn_7100c9e960. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBreakableTileGroup.h"
#include "types.h"

struct UnkQueryObj;
struct UnkTileQuery_A3_b64;

struct UnkWideBeamHitLink1_ovb0088 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;
};
struct UnkWideBeamHitLink0_ovb0088 {
    u8 pad_0x0[0x10];
    UnkWideBeamHitLink1_ovb0088* link;
};

struct UnkWideBeamHit_ovb0088 {
    UnkWideBeamHitLink0_ovb0088* link;
    u8 pad_0x8[0xc];
    u8 point[0xc];   // 0x14
    u8 normal[0x1c];  // 0x20
    u8 flags;         // 0x3c
};
static_assert(__builtin_offsetof(UnkWideBeamHit_ovb0088, flags) == 0x3c, "layout");

bool fn_7100869734(void* self, UnkWideBeamHit_ovb0088* hit) {
    if (!hit->link || !hit->link->link)
        return false;
    base::reflection::UnkReflObject* group = hit->link->link->object;
    if (!group || !fn_710009d50c(group->vf_0x0(), fn_7100ca0484()))
        return false;
    UnkTileQuery_A3_b64* tile;
    if (hit->flags & 0x20)
        tile = fn_7100ca1ecc(group, hit->point, hit->normal, true);
    else
        tile = fn_7100ca1ecc(group, hit->point, nullptr, true);
    if (!tile)
        return false;
    return fn_7100c9e960(reinterpret_cast<UnkQueryObj*>(tile)) == 1;
}
