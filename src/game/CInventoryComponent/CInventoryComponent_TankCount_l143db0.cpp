// CInventoryComponent unit: how many of a kind of tank (or tank shard) the inventory holds, read from its
// dictionary of item amounts; 0 when the item has no amount. Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CRntDictionary;
using base::global::CStrId;

struct UnkInventory_7101143db0 {
    u8 pad_0x0[0x40];
    CRntDictionary<CStrId, f32*> amounts;  // 0x40
};
static_assert(__builtin_offsetof(UnkInventory_7101143db0, amounts.buckets) == 0x50, "");

// The item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline s64 Amount(UnkInventory_7101143db0* self, UnkPropertyEx<>& item) {
    f32* const* amount = self->amounts.Find(ItemId(item).id);
    if (!amount || !*amount)
        return 0;
    return (s64)**amount;
}

s64 fn_7101143db0(UnkInventory_7101143db0* self, u64 kind) {
    switch (kind) {
    case 0:
        return Amount(self, g_unk_717275a2d8);  // ITEM_ENERGY_TANK_SHARDS
    case 1:
        return Amount(self, g_unk_717275a318);  // ITEM_ENERGY_TANKS
    case 2:
        return Amount(self, g_unk_717275a398);  // ITEM_MISSILE_PLUS_TANKS
    case 3:
        return Amount(self, g_unk_717275a358);  // ITEM_MISSILE_TANKS
    case 4:
        return Amount(self, g_unk_717275a3d8);  // ITEM_POWER_BOMB_TANKS
    }
    return 0;
}
