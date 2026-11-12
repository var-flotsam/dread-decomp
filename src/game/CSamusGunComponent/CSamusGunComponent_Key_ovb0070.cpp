// CSamusGunComponent unit (unit 1315 of tools/tu_map.py). Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCameraComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);

static inline const UnkQualifiedId& GetQualifiedId_ovb0070(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkEnergyOwner_ovb0070 {
    u8 unk_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkEnergyGun_ovb0070 {
    u8 unk_0x0[0x30];
    UnkEnergyOwner_ovb0070* owner;
    u8 unk_0x38[0xf0];
    bool field_0x128;
};

// A key chosen like fn_7100859798's: with ITEM_CENTRAL_UNIT_ENERGY and the aim camera, one of two by
// the flag at +0x128, the second only while ITEM_CENTRAL_UNIT_DECAYED_ENERGY is missing.
u64 fn_710085961c(UnkEnergyGun_ovb0070* self) {
    u64 key = 0xa218d351a57ec697ull;
    UnkInventory* inventory = self->owner->inventory;
    if (!inventory)
        return key;
    if (fn_71011428bc(inventory, GetQualifiedId_ovb0070(g_unk_717275ac18))) {
        key = 0xbe3a59363755d886ull;
        if (void* player = fn_7100defcec(g_unk_7101d47310)) {
            UnkReflObject* object = fn_71010e37f4(player);
            if (object && fn_710009d50c(object->vf_0x0(), fn_7100720464()) &&
                !reinterpret_cast<const bool*>(object)[0x240]) {
                if (self->field_0x128) {
                    UnkInventory* inv = self->owner->inventory;
                    if (!inv || !fn_71011428bc(inv, GetQualifiedId_ovb0070(g_unk_717275ac58)))
                        key = 0x94a143a64e136055ull;
                } else {
                    key = 0x635f98bb8b127f52ull;
                }
            }
        }
    }
    return key;
}
