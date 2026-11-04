// behaviortree::CScaredAction's unit (tools/tu_map.py): the end step runs the base (fn_7100394bd4),
// drops the weak handle at +0x50 (unlinking +0x38 from its target first), then, when the owner's
// (+0x18) sub-object at +0x220 accepts the class of fn_7100c49938 and the state at +0x30 is 2 or 3,
// sends it a fixed hash with mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h); it clears the state.
// Names are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkScaredOwner_b55 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkScaredAction_b55 {
    u8 pad_0x0[0x18];
    UnkScaredOwner_b55* owner;
    u8 pad_0x20[0x10];
    u32 field_0x30;
    u8 pad_0x34[0x4];
    u8 field_0x38[0x18];
    UnkWeakHandle* field_0x50;
};

void fn_7100390034(UnkScaredAction_b55* self) {
    fn_7100394bd4(self);
    if (UnkWeakHandle* handle = self->field_0x50) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x38);
        ResetRef(&self->field_0x50);
    }
    if (self->owner)
        if (UnkAnimationComponent* target = self->owner->field_0x220)
            if (fn_710117b348(target, fn_7100c49938()) && (self->field_0x30 & ~1u) == 2)
                UnkSpitSend(target, 0x40cef973c887bdc4, 0xf4061ff25a2163d6);
    self->field_0x30 = 0;
}
