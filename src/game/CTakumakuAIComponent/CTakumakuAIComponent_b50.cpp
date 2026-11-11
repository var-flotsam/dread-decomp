// CTakumakuAIComponent unit (tools/tu_map.py). fn_71006d4940, a message handler override: after the
// base AI component's handler (fn_710036974c), when the message's flag (+8) is set the owner's (+0x30)
// animation component (+0x220) gets the unit's id (fn_710117abfc, hash 0xbb2ee0d16add63b8); when it
// is clear and the animation plays a CCharClassAttackTrack (fn_7100ceda9c), +0x14750
// is cleared. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// As src/game/CSclawkAIComponent/CSclawkAIComponent_b44.cpp declares it.
struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;

// As src/game/CAutsniperShootAttack/fn_71003d5a14.cpp declares it.

extern char g_unk_7172709c30[] __attribute__((visibility("hidden")));

struct UnkTakumakuOwner_b50 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkTakumakuMessage_b50 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

struct UnkTakumakuAI_b50 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner_b50* owner;
    u8 pad_0x38[0x14750 - 0x38];
    u32 field_0x14750;
};

u32 fn_71006d4940(UnkTakumakuAI_b50* self, UnkTakumakuMessage_b50* msg) {
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    if (msg->field_0x8) {
        if (animation)
            fn_710117abfc(animation, 1, g_unk_7172709c30, 0xbb2ee0d16add63b8ull);
    } else if (animation && fn_710117b348(animation, fn_7100ceda9c())) {
        self->field_0x14750 = 0;
    }
    return result;
}
