// CInfesterAIComponent (the name from reflection, tools/tu_map.py): a check run while the owner's
// component at +0x278 (fn_7101136308) is active. When that component's +0x1a0 is the AI's own
// entry at +0x14758 the flag at +0x146de is set; otherwise the flag at +0x146df is set when the
// owner's character class has a CCharClassPushCanChangePrefixTrack (fn_710117b348). Names and
// layout are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CAttackComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkInfesterOwnerSlot_b44a2 {
    u8 pad_0x0[0x1a0];
    void* field_0x1a0;
};

struct UnkInfesterAI_b44a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146de - 0x38];
    bool field_0x146de;
    bool field_0x146df;
    u8 pad_0x146e0[0x14758 - 0x146e0];
    u8 field_0x14758[8];
};

void fn_71005cb394(UnkInfesterAI_b44a2* self) {
    void* slot = *reinterpret_cast<void**>(reinterpret_cast<char*>(self->owner) + 0x278);
    if (!fn_7101136308(slot))
        return;
    auto* s = *reinterpret_cast<UnkInfesterOwnerSlot_b44a2**>(reinterpret_cast<char*>(self->owner) + 0x278);
    if (s->field_0x1a0 == self->field_0x14758) {
        self->field_0x146de = true;
        return;
    }
    if (auto* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220)) {
        if (fn_710117b348(cls, fn_7100c46ce8()))
            self->field_0x146df = true;
    }
}
