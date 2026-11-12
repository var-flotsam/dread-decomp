// A drop component check (tools/tu_map.py: the CDropComponent unit): when the actor's life pair at
// +0x1b8 (+0x50, +0x54) is full and the inventory holds the most missiles it can
// (ITEM_WEAPON_MISSILE_CURRENT >= ITEM_WEAPON_MISSILE_MAX), sets the fourth weight of the drop table
// at +0x14728 so that it takes the tunable's share (+0x40 of the drop tunable) of the total. Names
// and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkInventory;
extern UnkTunable g_unk_717271b668;

struct UnkDropLife_b46a2 {
    u8 pad_0x0[0x50];
    f32 current;
    f32 max;
};

struct UnkDropActor_b46a2 {
    u8 pad_0x0[0x1b8];
    UnkDropLife_b46a2* life;
};

struct UnkDrop_b46a2 {
    u8 pad_0x0[0x14728];
    f32 weights[4];
};

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_b46a2(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_71007c46b4(UnkDrop_b46a2* self, UnkDropActor_b46a2* actor, UnkInventory* inventory) {
    UnkDropLife_b46a2* life = actor->life;
    if (!life)
        return;
    if (!(life->max >= life->current))
        return;
    f32 missiles = fn_7101142928(inventory, ItemId_b46a2(g_unk_717275a1d8));
    f32 max_missiles = fn_7101142928(inventory, ItemId_b46a2(g_unk_717275a218));
    if (!(missiles >= max_missiles))
        return;
    f32* weights = self->weights;
    f32 total = weights[1] + weights[0] + weights[2];
    self->weights[3] = total / (1.0f - *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_717271b668) + 0x40)) - total;
}
