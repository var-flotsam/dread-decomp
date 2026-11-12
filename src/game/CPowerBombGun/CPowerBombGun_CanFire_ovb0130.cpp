// CPowerBombGun's unit (tools/tu_map.py, unit 1313). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CBombGun.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2734.h"
#include "types.h"

using base::global::CStrId;

struct UnkInventory;

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid_ovb0130(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline UnkSubAreaGuiCache* GuiCache_ovb0130() {
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return cache;
}

struct UnkPowerBombGun_ovb0130 {
    u8 pad_0x0[0x1880];
    UnkQualifiedId ammo;  // 0x1880
};

// fn_71008565a8: whether the power bomb can be fired: with ammunition or the hyper suit
// (ITEM_HYPER_SUIT), and while the sub-area GUI cache counts the id at +0x1c68 of the id table.
bool fn_71008565a8(UnkPowerBombGun_ovb0130* self) {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return false;
    if (!(fn_7101142928(inventory, self->ammo) > 0.0f) && !fn_71011428bc(inventory, Qid_ovb0130(g_unk_717275a418)))
        return false;
    UnkSubAreaGuiCache* cache = GuiCache_ovb0130();
    return fn_710119db04(cache, &GameManagerIdTable().field_0x1c68) > 0;
}
