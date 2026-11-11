// CQuetzoaChargeAttack: fn_7100644d0c ends the charge (fn_7100644780) when `target` is the actor the
// owner (+0x8) acts against (fn_7100d7c990), unless the owner's part at +0x2c8 holds
// (fn_71007f9f20). Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkQuetzoaChargeAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_7100644780(UnkQuetzoaChargeAttack* self);

void fn_7100644d0c(UnkQuetzoaChargeAttack* self, void*, UnkActor* target) {
    void* part = *reinterpret_cast<void**>(reinterpret_cast<char*>(self->owner) + 0x2c8);
    bool held = false;
    if (part)
        held = fn_71007f9f20(part);
    UnkActor* against = fn_7100d7c990(self->owner);
    if (against == target && !held)
        fn_7100644780(self);
}
