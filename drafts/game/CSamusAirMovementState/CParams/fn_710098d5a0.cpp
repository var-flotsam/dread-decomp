// CSamusAirMovementState::CParams's unit (tools/tu_map.py unit 1455): whether another jump may start in the
// air. The movement (+0x38) must allow it (fn_71009e75e8) with nothing left at +0x51ac, and the game manager
// must not be in its blocked state (+0xc8 or +0x10c, the byte at +0xc4 set and the one at +0xc6 clear). With
// the flag at +0x23 of g_unk_717273afe0 it may; otherwise, with ITEM_DOUBLE_JUMP (and the manager's byte at
// +0x430 clear) or ITEM_SPACE_JUMP in the owner's (+0x30) inventory (+0x270), the player's state
// fn_7100df5874 decides. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;
struct UnkPlayer;

// As src/lifted declares it; the byte at +0x23 is a flag.
extern char g_unk_717273afe0[];

struct UnkAirMovement_l98d5a0 {
    u8 pad_0x0[0x51ac];
    f32 field_0x51ac;
};

struct UnkAirOwner_l98d5a0 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkAirParams_l98d5a0 {
    u8 pad_0x0[0x30];
    UnkAirOwner_l98d5a0* owner;         // 0x30
    UnkAirMovement_l98d5a0* movement;  // 0x38
};

inline bool Has_l98d5a0(UnkInventory* inventory, UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return fn_71011428bc(inventory, qid);
}

inline bool IsUnblocked_l98d5a0(UnkGameManager* manager) {
    const u8* bytes = reinterpret_cast<const u8*>(manager);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && bytes[0xc4])
        return bytes[0xc6] != 0;
    return true;
}

bool fn_710098d5a0(UnkAirParams_l98d5a0* self) {
    if (!fn_71009e75e8(reinterpret_cast<UnkFlagged*>(self->movement)))
        return false;
    if (self->movement->field_0x51ac > 0.0f)
        return false;
    bool allowed = IsUnblocked_l98d5a0(g_unk_7101d47310);
    if (!allowed || g_unk_717273afe0[0x23])
        return allowed;
    if (Has_l98d5a0(self->owner->inventory, g_unk_717275a898) && !reinterpret_cast<const u8*>(g_unk_7101d47310)[0x430])
        return fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    if (Has_l98d5a0(self->owner->inventory, g_unk_717275a958))
        return fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    return true;
}
