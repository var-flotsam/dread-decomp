// CMissileBaseGun's unit (tools/tu_map.py): the amount, as an integer, of the gun's ammo item
// (+0x1880) in the player's inventory (fn_7100df55e4 of the game manager); 0 without an inventory
// or with the empty id. Names are placeholders.
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkInventory;
void* fn_7100df55e4(UnkGameManager*);
f32 fn_7101142928(UnkInventory* inventory, const UnkQualifiedId& item);

struct UnkMissileBaseGun_b45a2 {
    u8 pad_0x0[0x1880];
    UnkQualifiedId ammo;
};

s32 fn_710084f828(UnkMissileBaseGun_b45a2* self) {
    auto* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return 0;
    if (self->ammo.id.data == &g_unk_7101d47108->empty)
        return 0;
    return static_cast<s32>(fn_7101142928(inventory, self->ammo));
}
