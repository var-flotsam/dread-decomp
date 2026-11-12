// CSecondaryGun unit. fn_710085ccf4: the player's amount of the gun's ammo item (its qualified id at
// +0x1890, read with fn_7101142928 from the inventory), or 1 while the gun has no item. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;

struct UnkSelf_sg41 {
    u8 pad_0x0[0x1890];
    UnkQualifiedId ammo;  // 0x1890
};

f32 fn_710085ccf4(UnkSelf_sg41* self) {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (self->ammo.id.data != &g_unk_7101d47108->empty)
        return fn_7101142928(inventory, self->ammo);
    return 1.0f;
}
