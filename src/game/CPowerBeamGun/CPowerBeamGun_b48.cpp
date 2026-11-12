// CPowerBeamGun unit (tools/tu_map.py): fn_7100853674 takes the gun's shot flags (vtable slot 0x198),
// adds 0x8000000 when the shot asks for it (+4) and the owner's inventory holds the diffusion beam
// (ITEM_WEAPON_DIFFUSION_BEAM, fn_71011428bc), and hands them to slot 0x1d8. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkGunOwner_853674 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkShot_853674 {
    u32 field_0x0;
    bool field_0x4;
};

struct UnkPowerBeamGun_853674 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkGunOwner_853674* owner;

    u32 ShotFlags() { return reinterpret_cast<u32 (*)(UnkPowerBeamGun_853674*)>(vtable[0x198 / 8])(this); }
    void Fire(u32 flags) {
        reinterpret_cast<void (*)(UnkPowerBeamGun_853674*, u32)>(vtable[0x1d8 / 8])(this, flags);
    }
};

void fn_7100853674(UnkPowerBeamGun_853674* self, const UnkShot_853674* shot) {
    u32 flags = self->ShotFlags();
    if (shot->field_0x4) {
        if (UnkInventory* inventory = self->owner->inventory) {
            if (fn_71011428bc(inventory, GetQualifiedId(g_unk_717275a558)))
                flags |= 0x8000000;
        }
    }
    self->Fire(flags);
}
