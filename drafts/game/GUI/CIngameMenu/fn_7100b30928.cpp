// CIngameMenu (unit 1681): whether an item is still the best of its line, that is, the inventory holds
// none of the items that supersede it (beams, missiles, suits, jumps); true without an inventory.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

// The item's qualified id, built on first use.
static inline UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline bool Has(UnkInventory* inventory, UnkPropertyEx<>& item) {
    return fn_71011428bc(inventory, ItemId(item));
}

bool fn_7100b30928(UnkIngameMenu* self, const CStrId& item) {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return true;

    if (item == ItemId(g_unk_717275a5d8).id) {  // wide beam
        if (Has(inventory, g_unk_717275a6d8) || Has(inventory, g_unk_717275a658) || Has(inventory, g_unk_717275a718))
            return false;
    } else if (item == ItemId(g_unk_717275a6d8).id) {  // plasma beam
        if (Has(inventory, g_unk_717275a658) || Has(inventory, g_unk_717275a718))
            return false;
    } else if (item == ItemId(g_unk_717275a658).id) {  // wave beam
        return !Has(inventory, g_unk_717275a718);
    } else if (item == ItemId(g_unk_717275a758).id) {  // super missile
        if (Has(inventory, g_unk_717275a798))
            return false;
    } else if (item == ItemId(g_unk_717275a498).id) {  // varia suit
        if (Has(inventory, g_unk_717275a458) || Has(inventory, g_unk_717275a418))
            return false;
    } else if (item == ItemId(g_unk_717275a458).id) {  // gravity suit
        if (Has(inventory, g_unk_717275a418))
            return false;
    } else if (item == ItemId(g_unk_717275a898).id) {  // double jump
        if (Has(inventory, g_unk_717275a958))
            return false;
    }
    return true;
}
