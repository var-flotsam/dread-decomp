// CSamusGunComponent's unit (tools/tu_map.py): fn_7100859a20 and fn_710085a1e8, two more keys chosen by the
// central unit energy, the shape of fn_7100859798 (src/game/CSamusGunComponent/CSamusGunComponent_EnergyKey_b52.cpp)
// with the keys read from constants reached through the GOT: without ITEM_CENTRAL_UNIT_ENERGY in the owner's
// inventory (+0x30, +0x270) the first; with it the second, unless the player's (fn_7100defcec) object
// (fn_71010e37f4) is of the class fn_7100720464 names and not flagged (+0x240): then one of two by the flag at
// +0x128. Names and layouts are placeholders.
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

extern const u64 g_unk_710164adb8;
extern const u64 g_unk_710164adb0;
extern const u64 g_unk_710164ada0;
extern const u64 g_unk_710164ada8;
extern const u64 g_unk_710164ad98;
extern const u64 g_unk_710164ad90;
extern const u64 g_unk_710164ad80;
extern const u64 g_unk_710164ad88;

static inline const UnkQualifiedId& GetQualifiedId_A4_b76(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkEnergyOwner_A4_b76 {
    u8 unk_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkEnergyGun_A4_b76 {
    u8 unk_0x0[0x30];
    UnkEnergyOwner_A4_b76* owner;
    u8 unk_0x38[0xf0];
    bool field_0x128;
};

template <const u64& None, const u64& Some, const u64& Set, const u64& Clear>
__attribute__((always_inline)) inline u64 EnergyKey(UnkEnergyGun_A4_b76* self) {
    const u64* key = &None;
    UnkInventory* inventory = self->owner->inventory;
    if (!inventory)
        return *key;
    if (fn_71011428bc(inventory, GetQualifiedId_A4_b76(g_unk_717275ac18))) {
        key = &Some;
        if (void* player = fn_7100defcec(g_unk_7101d47310)) {
            UnkReflObject* object = fn_71010e37f4(player);
            if (object && fn_710009d50c(object->vf_0x0(), fn_7100720464()) &&
                !reinterpret_cast<const bool*>(object)[0x240])
                key = self->field_0x128 ? &Set : &Clear;
        }
    }
    return *key;
}

u64 fn_7100859a20(UnkEnergyGun_A4_b76* self) {
    return EnergyKey<g_unk_710164adb8, g_unk_710164adb0, g_unk_710164ada0, g_unk_710164ada8>(self);
}

u64 fn_710085a1e8(UnkEnergyGun_A4_b76* self) {
    return EnergyKey<g_unk_710164ad98, g_unk_710164ad90, g_unk_710164ad80, g_unk_710164ad88>(self);
}
