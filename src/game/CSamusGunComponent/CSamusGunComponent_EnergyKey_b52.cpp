// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): a key chosen by the central unit energy,
// fn_7100859798. Without the item ITEM_CENTRAL_UNIT_ENERGY in the owner's inventory (+0x30, +0x270;
// fn_71011428bc) the key is 0x51865f70508a80d2; with it, 0x305528a169513e9c, unless the player's
// (fn_7100defcec) object (fn_71010e37f4) is of the class fn_7100720464 names and not flagged
// (+0x240): then one of two keys by the flag at +0x128. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCameraComponent.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkInventory;

static inline const UnkQualifiedId& GetQualifiedId_b52(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkEnergyOwner_b52 {
    u8 unk_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkEnergyGun_b52 {
    u8 unk_0x0[0x30];
    UnkEnergyOwner_b52* owner;
    u8 unk_0x38[0xf0];
    bool field_0x128;
};

u64 fn_7100859798(UnkEnergyGun_b52* self) {
    u64 key = 0x51865f70508a80d2ull;
    UnkInventory* inventory = self->owner->inventory;
    if (!inventory)
        return key;
    if (fn_71011428bc(inventory, GetQualifiedId_b52(g_unk_717275ac18))) {
        key = 0x305528a169513e9cull;
        if (void* player = fn_7100defcec(g_unk_7101d47310)) {
            UnkReflObject* object = fn_71010e37f4(player);
            if (object && fn_710009d50c(object->vf_0x0(), fn_7100720464()) &&
                !reinterpret_cast<const bool*>(object)[0x240])
                key = self->field_0x128 ? 0x105414068e66aae1ull : 0xcd4b23c335d17ffbull;
        }
    }
    return key;
}
