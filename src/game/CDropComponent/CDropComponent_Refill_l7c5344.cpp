// CDropComponent unit (tools/tu_map.py unit 1223): for each supply the player lacks (life, missiles, and power
// bombs once owned), copies the two matching drop values into the two matching drop slots. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkDropLife_l7c5344 {
    u8 pad_0x0[0x50];
    f32 current;
    f32 max;
};

struct UnkDropOwner_l7c5344 {
    u8 pad_0x0[0x1c0];
    UnkDropLife_l7c5344* life;
};

struct UnkDropInventory_l7c5344 {
    u8 pad_0x0[0x30];
    UnkDropOwner_l7c5344* owner;
};

struct UnkDropSlot_l7c5344 {
    u8 pad_0x0[0x484];
    u32 value;

    void SetValue(u32 new_value) { value = new_value; }
};

struct UnkDropValues_l7c5344 {
    u8 pad_0x0[0x14];
    u32 values[6];
};

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_l7c5344(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

static inline f32 ItemAmount_l7c5344(UnkInventory* inventory, UnkPropertyEx<>& item) {
    return fn_7101142928(inventory, ItemId_l7c5344(item));
}

void CDropComponent_SetSlotValuesForLackingSupplies(UnkDropInventory_l7c5344* self, UnkDropSlot_l7c5344** slots, const UnkDropValues_l7c5344* drop) {
    auto* inventory = reinterpret_cast<UnkInventory*>(self);
    UnkDropLife_l7c5344* life = self->owner->life;
    if (life && !(life->max >= life->current)) {
        slots[0]->SetValue(drop->values[0]);
        slots[1]->SetValue(drop->values[1]);
    }
    f32 missiles = ItemAmount_l7c5344(inventory, g_unk_717275a1d8);      // ITEM_WEAPON_MISSILE_CURRENT
    f32 max_missiles = ItemAmount_l7c5344(inventory, g_unk_717275a218);  // ITEM_WEAPON_MISSILE_MAX
    if (!(missiles >= max_missiles)) {
        slots[2]->SetValue(drop->values[2]);
        slots[3]->SetValue(drop->values[3]);
    }
    bool has_power_bombs = fn_71011428bc(inventory, ItemId_l7c5344(g_unk_717275a818));  // ITEM_WEAPON_POWER_BOMB
    if (has_power_bombs) {
        f32 bombs = ItemAmount_l7c5344(inventory, g_unk_717275a258);      // ITEM_WEAPON_POWER_BOMB_CURRENT
        f32 max_bombs = ItemAmount_l7c5344(inventory, g_unk_717275a298);  // ITEM_WEAPON_POWER_BOMB_MAX
        if (!(bombs >= max_bombs)) {
            slots[4]->SetValue(drop->values[4]);
            slots[5]->SetValue(drop->values[5]);
        }
    }
}
