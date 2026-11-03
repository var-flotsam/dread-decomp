// CGun's unit (tools/tu_map.py unit 1302): whether the gun has ammo: true for the empty ammo id
// (+0x28), else whether the owner's (+0x18) inventory (+0x270, or the player's through
// fn_7100df55e4) holds a positive amount of it; false without an inventory. Names are placeholders.
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkInventory;
void* fn_7100df55e4(UnkGameManager*);
f32 fn_7101142928(UnkInventory* inventory, const UnkQualifiedId& item);

struct UnkGunOwner_b45a2 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkGun_b45a2 {
    u8 pad_0x0[0x18];
    UnkGunOwner_b45a2* owner;
    u8 pad_0x20[8];
    UnkQualifiedId ammo;  // 0x28
};

bool fn_7100835974(UnkGun_b45a2* self) {
    if (self->ammo.id.data == &g_unk_7101d47108->empty)
        return true;
    UnkInventory* inventory = self->owner->inventory;
    if (!inventory) {
        inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
        if (!inventory)
            return false;
    }
    return 0.0f < fn_7101142928(inventory, self->ammo);
}
