// CSabotoruAIComponent unit (tools/tu_map.py): the damage handler. The base handler (fn_7101105e44)
// answers unless the state at +0x14770 says otherwise; flagged hits are dropped (answer 2) unless the
// owner is frozen, and in mode 1 a hit the state rejects sends the owner's animation a fixed request.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAttackComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkSabotoruEvent_l679e38 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkSabotoruOwner_l679e38 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x30];
    void* attack;  // 0x258
    u8 pad_0x260[0x68];
    UnkFrostbiteComponent* frozen;  // 0x2c8
};

struct UnkSabotoruState_l679e38 {
    void** vtable;
    u8 pad_0x8[0x5f8];
    u32 field_0x600;

    bool Accepts(UnkSabotoruOwner_l679e38* owner, UnkSabotoruEvent_l679e38* event) {
        return reinterpret_cast<bool (*)(UnkSabotoruState_l679e38*, UnkSabotoruOwner_l679e38*,
                                         UnkSabotoruEvent_l679e38*)>(vtable[0xc0 / 8])(this, owner, event);
    }
};

struct UnkSabotoruAI_l679e38 {
    u8 pad_0x0[0x30];
    UnkSabotoruOwner_l679e38* owner;  // 0x30
    u8 pad_0x38[0x121f - 0x38];
    bool field_0x121f;
    u8 pad_0x1220[0x14770 - 0x1220];
    UnkSabotoruState_l679e38* state;  // 0x14770
    u32 mode;                         // 0x14778
};
static_assert(__builtin_offsetof(UnkSabotoruAI_l679e38, state) == 0x14770, "");

u32 fn_7100679e38(UnkSabotoruAI_l679e38* self, UnkSabotoruEvent_l679e38* event) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    UnkSabotoruState_l679e38* state = self->state;
    if (state->field_0x600 == 1)
        return result;
    if (event->flags & 0x40800) {
        UnkFrostbiteComponent* frozen = self->owner->frozen;
        if (frozen && fn_71007f9f20(frozen) && !fn_71007f9fd0(frozen))
            return result;
        event->flags = 0;
        return 2;
    }
    if (self->mode == 1) {
        if (result)
            return result;
        if (state->Accepts(self->owner, event) && !(event->flags & 0xa01846e)) {
            event->flags = 0;
            return 2;
        }
        UnkSabotoruOwner_l679e38* owner = self->owner;
        if (!owner)
            return 0;
        UnkFrostbiteComponent* frozen = owner->frozen;
        if (!frozen || fn_71007f9f20(frozen) || fn_71007f9fd0(frozen))
            return 0;
        UnkAnimationComponent* animation = self->owner->animation;
        if (animation && !fn_710117b348(animation, fn_7100c4f408()) && !fn_7101136308(self->owner->attack))
            base::reflection::refl_EmitU32(animation, 0x380af14de32f217f, 0xf4061ff25a2163d6, 2,
                                           &g_unk_710164b958, &g_unk_7172769740);
        return 0;
    }
    UnkFrostbiteComponent* frozen = self->owner->frozen;
    if (!frozen || !fn_71007f9f20(frozen))
        self->field_0x121f = true;
    return result;
}
