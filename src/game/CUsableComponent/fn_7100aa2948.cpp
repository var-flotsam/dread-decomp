// CUsableComponent's unit (tools/tu_map.py): whether the use animations are over: neither the user's
// animation (on the user's actor) nor the usable's own (on its owner) for the use kind at +0x10 of the
// record at +0x110 is still playing, and the user is not busy (its +0x260 in state 1 with a target).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CUsableComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkUsableBusyTarget_laa2948 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkUsableBusy_laa2948 {
    u8 pad_0x0[0xe8];
    u32 state;
    u8 pad_0xec[0x4];
    UnkUsableBusyTarget_laa2948* target;  // 0xf0
};

struct UnkUsableActor_laa2948 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    UnkUsableBusy_laa2948* busy;  // 0x260
};

struct UnkUsableUser_laa2948 {
    u8 pad_0x0[0x10];
    UnkUsableActor_laa2948* actor;
};

struct UnkUsableUse_laa2948 {
    u8 pad_0x0[0x10];
    u32 kind;
};

struct UnkUsableAnims_laa2948 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkUsableActor_laa2948* owner;  // 0x30
    u8 pad_0x38[0xb0];
    UnkUsableUser_laa2948* user;  // 0xe8
    u8 pad_0xf0[0x20];
    UnkUsableUse_laa2948* use;  // 0x110

    const CStrId& Anim(u32 slot) {
        return reinterpret_cast<const CStrId& (*)(UnkUsableAnims_laa2948*)>(vtable[slot / 8])(this);
    }

    // The animation the user plays for the current use kind.
    const CStrId& UserAnim() {
        const CStrId* anim = &g_unk_7101d470e0;
        if (use) {
            switch (use->kind) {
            case 1:
                anim = &Anim(0x1b8);
                break;
            case 4:
                anim = &Anim(0x198);
                break;
            case 5:
                anim = &Anim(0x1a0);
                break;
            case 6:
                anim = &Anim(0x1a8);
                break;
            case 7:
                anim = &Anim(0x1b0);
                break;
            }
        }
        return *anim;
    }

    // The animation the usable's owner plays for the current use kind.
    const CStrId& OwnAnim() {
        const CStrId* anim = &g_unk_7101d470e0;
        if (use) {
            switch (use->kind) {
            case 1:
                anim = &Anim(0x218);
                break;
            case 3:
                anim = &Anim(0x1f0);
                break;
            case 4:
                anim = &Anim(0x1f8);
                break;
            case 5:
                anim = &Anim(0x208);
                break;
            case 6:
                anim = &Anim(0x200);
                break;
            case 7:
                anim = &Anim(0x210);
                break;
            }
        }
        return *anim;
    }
};
static_assert(__builtin_offsetof(UnkUsableAnims_laa2948, use) == 0x110, "UnkUsableAnims_laa2948 layout");

// Whether `animation` is done with `anim`: not while the user is busy, nor while it still plays it.
static inline bool AnimDone(bool busy, UnkAnimationComponent* animation, const CStrId& anim) {
    return !busy && !(animation && anim.data->Key() != (u64)-1 && fn_710117ae38(animation, anim.data->Key(), true));
}

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
bool fn_7100aa2948(UnkUsableComponent_A6_b62* component) {
    auto* self = reinterpret_cast<UnkUsableAnims_laa2948*>(component);
    if (!self->user)
        return false;
    UnkUsableActor_laa2948* actor = self->user->actor;
    if (!actor)
        return false;
    UnkUsableBusy_laa2948* b = actor->busy;
    bool busy = b && b->state == 1 && b->target && b->target->field_0x10;
    bool user_done = AnimDone(busy, actor->animation, self->UserAnim());
    bool own_done = AnimDone(busy, self->owner->animation, self->OwnAnim());
    return user_done && own_done;
}
