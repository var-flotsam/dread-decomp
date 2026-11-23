// behaviortree::CSetEndAnimAction's unit (tools/tu_map.py unit 454): the action's update. The first time it
// sends the owner's animation the target animation (bForce as the mode, bSyncWithCurrent picking the
// descriptor); then, with bWaitAnimEnd, it runs (2) while that animation still plays, or while the owner is
// frozen with bWaitIfFrozen, and is done (5); 6 when nothing could be started. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;

struct UnkSetEndAnimOwner_l371808 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0xa0];
    void* frozen;  // 0x2c8
};
static_assert(__builtin_offsetof(UnkSetEndAnimOwner_l371808, frozen) == 0x2c8, "layout");

struct UnkSetEndAnimAction_l371808 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkSetEndAnimOwner_l371808* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool bForce;            // 0x2c
    bool bWaitAnimEnd;      // 0x2d
    bool bWaitIfFrozen;     // 0x2e
    bool started;           // 0x2f
    bool bSyncWithCurrent;  // 0x30

    // The animation to wait for.
    const CStrId* GetAnim() {
        return reinterpret_cast<const CStrId* (*)(UnkSetEndAnimAction_l371808*)>(vtable[0xa0 / 8])(this);
    }
    // The animation to set (in and out), and whether there is one.
    bool GetTargetAnim(const CStrId** anim, bool* valid) {
        return reinterpret_cast<bool (*)(UnkSetEndAnimAction_l371808*, const CStrId**, bool*)>(
            vtable[0xa8 / 8])(this, anim, valid);
    }
};

static inline bool IsSet(const CStrId* id) {
    return id->data != &g_unk_7101d47108->empty;
}

s32 fn_7100371808(UnkSetEndAnimAction_l371808* self) {
    const u64 track = 0xf4061ff25a2163d6;
    const CStrId* anim = self->GetAnim();
    if (!self->started) {
        UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
        bool valid = false;
        const CStrId* target = anim;
        if (!self->GetTargetAnim(&target, &valid))
            return 2;
        if (valid && target && IsSet(target)) {
            u64 hash = target->data->Key();
            u32 mode = self->bForce;  // only the id word of the argument
            const void* desc = self->bSyncWithCurrent ? static_cast<const void*>(&g_unk_7172769770)
                                                      : static_cast<const void*>(&g_unk_7172769740);
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            self->started = fn_7101176690(animation, hash, track, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode),
                                          &g_unk_710164b958, desc, &mask) ||
                            fn_710117ae38(animation, hash, false);
        }
    }
    if (!self->started)
        return 6;
    if (!self->bWaitAnimEnd)
        return 5;
    if (!IsSet(anim))
        return 6;
    if (self->owner) {
        UnkAnimationComponent* animation = self->owner->animation;
        if (animation && fn_7101177ae8(animation, track)->data == anim->data && !fn_71011782ec(animation, track))
            return 2;
    }
    if (self->bWaitIfFrozen && self->owner && self->owner->frozen && fn_71007f9f20(self->owner->frozen))
        return 2;
    return 5;
}
