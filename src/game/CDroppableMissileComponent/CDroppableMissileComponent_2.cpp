// In the CDroppableMissileComponent unit: tops up an item in the inventory of `owner` (+0x270):
// the current amount of the item at +0xb8 plus 5, capped by the item at +0xa8 and never below
// zero, through fn_7101142a28 (as lua_bind::GainAllItemsAndPowerUps calls it). Names and layouts
// are placeholders.
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"

struct UnkInventory;

struct UnkDroppableMissileOwner {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkDroppableMissileComponent {
    u8 pad_0x0[0xa8];
    UnkQualifiedId max_item;
    u8 pad_0xa8_end[0xb8 - 0xa8 - sizeof(UnkQualifiedId)];
    UnkQualifiedId item;
};

void fn_71007bc738(UnkDroppableMissileComponent* self, UnkDroppableMissileOwner* owner) {
    if (!owner)
        return;
    UnkInventory* inventory = owner->inventory;
    if (!inventory)
        return;
    f32 amount = fn_7101142928(inventory, self->item) + 5.0f;
    f32 max = fn_7101142928(inventory, self->max_item);
    f32 value = 0.0f;
    if (!(amount <= 0.0f))
        value = amount >= max ? max : amount;
    fn_7101142a28(inventory, self->item, true, true, true, value, 0.0f);
}
