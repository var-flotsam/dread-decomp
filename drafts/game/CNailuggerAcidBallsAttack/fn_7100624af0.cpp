// CNailuggerAcidBallsAttack unit (tools/tu_map.py): the attack's start. After the base start, tells the owner's
// animation (+0x220) which track to play (one id when it has a turn track, another otherwise, the first also
// stopping the owner's navigation), keeps the owner's Nailong AI component and animation, and hooks the
// callback fn_7100625b98 (link at +0x30, handle at +0x48) up to the animation. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_7100625b98();

// The kind-5 block handed to the navigation (fn_71006f4b0c), with the fields this start sets.
struct UnkAcidBallsParams_l624af0 {
    void** vtable;
    u64 field_0x8;
    f32 speed;  // 0x10
    bool speed_set;
    u8 pad_0x15[3];
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 field_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;
};

struct UnkAcidBallsLink_l624af0 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkAcidBallsOwner_l624af0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* navigation;  // 0x298
};

struct UnkAcidBallsAttack_l624af0 {
    void** vtable;
    UnkAcidBallsOwner_l624af0* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkAcidBallsLink_l624af0 link;  // 0x30
    UnkWeakHandle* handle;          // 0x48
    u32 state;                      // 0x50
    u8 pad_0x54[4];
    UnkAnimationComponent* animation;  // 0x58
    UnkReflObject* ai;                 // 0x60
    u32 field_0x68;
    bool field_0x6c;
    u8 pad_0x6d[3];
    u64 field_0x70;
    u64 field_0x78;
};

void fn_7100624af0(UnkAcidBallsAttack_l624af0* self) {
    const u64 kind = 0xf4061ff25a2163d6ull;
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->state = 0;
    UnkAnimationComponent* animation = self->owner->animation;
    switch (static_cast<int>(fn_710117b348(animation, fn_7100d2b3a8()))) {
    case 1:
        base::reflection::UnkReflTypeIdArg turn_mode;
        base::reflection::UnkReflTypeIdArg turn_mask;
        UnkAcidBallsParams_l624af0 params;
        turn_mode.id = 10;
        turn_mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0xb3feb6d516806342ull, kind, &turn_mode, &g_unk_710164b958, &g_unk_7172769740,
                      &turn_mask);
        params.field_0x30 = false;
        params.field_0x20 = 0;
        params.field_0x28 = 0;
        params.field_0x39 = false;
        params.field_0x3b = false;
        params.field_0x3d = false;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x8 = 0;
        params.kind = 5;
        params.field_0x18 = 0;
        params.speed = 1.0f;
        params.field_0x1c = true;
        params.speed_set = true;
        fn_71006f4b0c(self->owner->navigation, &params, 1);
        break;
    default:
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        mode.id = 2;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(animation, 0xcb24643fce5b5852ull, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        break;
    }

    self->field_0x6c = false;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    UnkReflObject* ai = self->owner->ai;
    self->ai = ai && fn_710009d50c(ai->vf_0x0(), fn_710061f464()) ? ai : nullptr;
    self->animation = self->owner->animation;
    if (!self->owner->animation)
        return;

    UnkAcidBallsLink_l624af0* link = &self->link;
    link->context = self;
    link->callback = fn_7100625b98;
    UnkWeakTarget* track = reinterpret_cast<UnkWeakTarget*>(self->owner->animation);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        UnkWeakHandle* old = self->handle;
        if (old) {
            if (--old->ref_count == 0)
                old->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        CAbilityGhostAura_AssignWeakHandleToRef(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
}
