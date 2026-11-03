// In the CDroppablePowerBombComponent unit: the same top-up as fn_71007bc738
// (CDroppableMissileComponent_2.cpp): the amount of the item at +0xb8 plus 5, capped by the item at
// +0xa8 and never below zero. Names and layouts are placeholders.
#include "game/UnkGameBlackboard.h"

struct UnkInventory;
f32 fn_7101142928(UnkInventory* inventory, const UnkQualifiedId& item);
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount,
                   f32 extra);

struct UnkDroppablePowerBombOwner {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkDroppablePowerBombComponent {
    u8 pad_0x0[0xa8];
    UnkQualifiedId max_item;
    u8 pad_0xa8_end[0xb8 - 0xa8 - sizeof(UnkQualifiedId)];
    UnkQualifiedId item;
};

void fn_71007bd808(UnkDroppablePowerBombComponent* self, UnkDroppablePowerBombOwner* owner) {
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
