// CSecondaryGun unit. fn_710085cc94: the player's amount of the gun's ammo item (its qualified id at
// +0x1880, read with fn_7101142928 from the inventory), or 1 while the gun has no item. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkInventory;
void* fn_7100df55e4(UnkGameManager*);
f32 fn_7101142928(UnkInventory*, const UnkQualifiedId&);

struct UnkSelf_sg42 {
    u8 pad_0x0[0x1880];
    UnkQualifiedId ammo;  // 0x1880
};

f32 fn_710085cc94(UnkSelf_sg42* self) {
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (self->ammo.id.data != &g_unk_7101d47108->empty)
        return fn_7101142928(inventory, self->ammo);
    return 1.0f;
}
