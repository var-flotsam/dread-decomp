// CAbility unit (tools/tu_map.py, the name from reflection): true when the ability's item id (+0x20 of
// the object at +0xc8) is empty, else when the owner's inventory (+0x270 of +0x30 of the object at +0xc0)
// holds more than none of it (fn_7101142928). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "types.h"

struct UnkInventory;

struct UnkAbilityItem_b66a2 {
    u8 pad_0x0[0x20];
    UnkQualifiedId id;
};

struct UnkAbilityActor_b66a2 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkAbilityOwner_b66a2 {
    u8 pad_0x0[0x30];
    UnkAbilityActor_b66a2* actor;
};

struct UnkAbility_b66a2 {
    u8 pad_0x0[0xc0];
    UnkAbilityOwner_b66a2* owner;
    UnkAbilityItem_b66a2* item;
};

bool fn_71002f927c(UnkAbility_b66a2* self) {
    UnkAbilityItem_b66a2* item = self->item;
    if (item->id.id.data == &g_unk_7101d47108->empty) {
        return true;
    }
    UnkInventory* inventory = self->owner->actor->inventory;
    if (!inventory) {
        return false;
    }
    return fn_7101142928(inventory, item->id) > 0.0f;
}
