// CSamusAlternativeActionPlayerComponent unit (tools/tu_map.py). fn_7100712fe4 tells whether an
// action (its id at +0x30 of the request's +0x0) is allowed: one id needs the power bomb item
// (ITEM_WEAPON_POWER_BOMB) in the owner's inventory (+0x270 of +0x30), another needs it absent;
// anything else, or no inventory, is allowed. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;

// An item's qualified id, built on first use (as in src/game/CMeleeComponent/fn_710092f204.cpp).
static inline const UnkQualifiedId& GetQualifiedId_b50(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkActionDef_b50 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkActionRequest_b50 {
    UnkActionDef_b50* def;
};

struct UnkActionOwner_b50 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkAlternativeAction_b50 {
    u8 pad_0x0[0x30];
    UnkActionOwner_b50* owner;
};

bool fn_7100712fe4(UnkAlternativeAction_b50* self, UnkActionRequest_b50* request) {
    u64 id = request->def->id;
    if (id == 0x53e86f47303934d1ull) {
        if (UnkInventory* inventory = self->owner->inventory)
            return !fn_71011428bc(inventory, GetQualifiedId_b50(g_unk_717275a818));
    } else if (id == 0x144215ae9bdecbe5ull) {
        if (UnkInventory* inventory = self->owner->inventory)
            return fn_71011428bc(inventory, GetQualifiedId_b50(g_unk_717275a818));
    }
    return true;
}
