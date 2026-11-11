// CChozoCommanderAIComponent's unit (tools/tu_map.py): false unless fn_710110c85c agrees; then true, except
// while the owner's slot at +0x278 is held by this component's record at +0x14bf0, when the answer is
// fn_710092b6f8 of the object at +0x2f8 of the actor the owner acts against (fn_7100d7c990); false
// without either.
// Names and layouts are placeholders.
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkCommanderSlot_A2_b68 {
    u8 pad_0x0[0x1a0];
    void* holder;
};

struct UnkCommanderOwner_A2_b68 {
    u8 pad_0x0[0x278];
    UnkCommanderSlot_A2_b68* slot;
};

struct UnkCommanderTarget_A2_b68 {
    u8 pad_0x0[0x2f8];
    void* field_0x2f8;
};

struct UnkCommanderAI_A2_b68 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_A2_b68* owner;
    u8 pad_0x38[0x14bf0 - 0x38];
    u8 record[0x10];
};

bool fn_710110c85c(UnkCommanderAI_A2_b68* self);

bool fn_71004532a4(UnkCommanderAI_A2_b68* self) {
    if (!fn_710110c85c(self))
        return false;
    UnkCommanderOwner_A2_b68* owner = self->owner;
    UnkCommanderSlot_A2_b68* slot = owner->slot;
    if (!slot)
        return true;
    if (slot->holder != self->record)
        return true;
    if (UnkCommanderTarget_A2_b68* target =
            reinterpret_cast<UnkCommanderTarget_A2_b68*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(owner)))) {
        if (void* p = target->field_0x2f8)
            return fn_710092b6f8(p);
    }
    return false;
}
