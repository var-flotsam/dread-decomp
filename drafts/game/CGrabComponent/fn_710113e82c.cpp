// CGrabComponent unit (tools/tu_map.py): the per-frame grab update. Drops the target (+0xd8) once
// it can no longer be grabbed, times a pending grab (+0xe8) and cancels it when it is no longer
// possible, then steps the grab of the actor it holds (+0xd0, slowed down with slow motion where
// the class allows it) and of the actor holding it (+0xc8). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CBombGun.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// As src/game/CGrabComponent/CGrabComponent_Grab_ovb0057.cpp defines them.
struct UnkGrabComponent_ovb0057;
struct UnkGrabActor_ovb0057;
struct UnkGrabRequest_ovb0057;
UnkGrabRequest_ovb0057* CGrabComponent_FindRequestAcceptingActor(UnkGrabComponent_ovb0057* self, UnkGrabActor_ovb0057* other);
void CGrabComponent_RequestAnimAndClearTarget(UnkGrabComponent_ovb0057* self);
void fn_710113b8cc(UnkGrabComponent_ovb0057* self);
void fn_710113c304(UnkGrabComponent_ovb0057* self);
void fn_710113def8(UnkGrabComponent_ovb0057* self, f32 dt);
bool fn_710119dc88(UnkSubAreaGuiCache* self, void* actor, bool flag);
f32 fn_71010d6858();

// The slow-motion flags as bytes of the game manager's word at +0xc4.
struct UnkSlomoFlags {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};

struct UnkGrabState_b3e82c {
    u8 pad_0x0[0x70];
    s32 kind;
};

struct UnkGrabActor_b3e82c {
    u8 pad_0x0[0x1b8];
    UnkGrabState_b3e82c* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x184];
    f32 field_0x3ac;
};
static_assert(__builtin_offsetof(UnkGrabActor_b3e82c, field_0x3ac) == 0x3ac, "UnkGrabActor_b3e82c layout");

// A counted link to a grabbed or grabbing actor.
struct UnkGrabLink_b3e82c : UnkRefObject {
    UnkGrabActor_b3e82c* target;  // 0x10
};

struct UnkGrabComponent_b3e82c {
    u8 pad_0x0[0x30];
    UnkGrabActor_b3e82c* owner;
    u8 pad_0x38[0x90];
    UnkGrabLink_b3e82c* field_0xc8;  // the actor holding this one
    UnkGrabLink_b3e82c* field_0xd0;  // the actor this one holds
    UnkGrabLink_b3e82c* target;      // 0xd8
    bool target_set;                 // 0xe0
    u8 pad_0xe1[7];
    s32 state;                       // 0xe8: 1 while a grab is pending
    u8 pad_0xec[4];
    UnkWeakHandle* grabbed;          // 0xf0
    u8 pad_0xf8[0x88];
    u64 field_0x180;
    u32 field_0x188;
    u8 pad_0x18c[0x1c];
    f32 field_0x1a8;  // how long the grab has been pending
    u8 pad_0x1ac[0x2c];
    bool field_0x1d8;
    UnkGrabComponent_ovb0057* Grab() { return reinterpret_cast<UnkGrabComponent_ovb0057*>(this); }
};
static_assert(__builtin_offsetof(UnkGrabComponent_b3e82c, field_0x1d8) == 0x1d8, "UnkGrabComponent_b3e82c layout");

static inline UnkGrabActor_b3e82c* Target(UnkGrabLink_b3e82c* link) {
    return link ? link->target : nullptr;
}

// Whether the actor's class lets it be grabbed (no animation component: yes).
static inline bool CanBeGrabbed(UnkGrabActor_b3e82c* actor) {
    UnkAnimationComponent* anim = actor->field_0x220;
    return !anim || fn_710117b348(anim, fn_7100cef3e8());
}

static inline UnkSubAreaGuiCache* GuiCache() {
    UnkSubAreaGuiCache* cache = g_unk_71727254f8;
    if (!cache) {
        cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
        fn_710119d484(cache);
        g_unk_71727254f8 = cache;
    }
    return cache;
}

// The grab's time step: slowed down while slow motion is on, if the actor's class allows it.
static inline f32 GrabDt(UnkGrabActor_b3e82c* actor, f32 dt) {
    UnkGameManager* manager = g_unk_7101d47310;
    UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && slomo->enabled && !slomo->paused &&
        fn_710117b348(actor->field_0x220, fn_7100bfd9c8()))
        dt *= fn_71010d6858();
    return dt;
}

void fn_710113e82c(UnkGrabComponent_b3e82c* self, f32 dt) {
    if (self->target_set) {
        bool grabbable = Target(self->target) && CanBeGrabbed(self->owner) &&
                         CGrabComponent_FindRequestAcceptingActor(self->Grab(), reinterpret_cast<UnkGrabActor_ovb0057*>(Target(self->target)));
        if (grabbable != self->target_set) {
            self->target_set = grabbable;
            if (!grabbable)
                CGrabComponent_RequestAnimAndClearTarget(self->Grab());
        }
    }

    if (!Target(self->field_0xc8) && !Target(self->field_0xd0)) {
        self->field_0x1a8 += dt;
        if (self->state == 1 && self->grabbed && self->grabbed->target) {
            if (self->owner->field_0x3ac >= 0.0f || fn_710119dc88(GuiCache(), self->owner, true) ||
                (self->owner->field_0x1b8 && self->owner->field_0x1b8->kind == 2) || Target(self->field_0xc8) ||
                Target(self->field_0xd0)) {
                // The grab can no longer happen: cancel it.
                self->state = 0;
                ResetRef(&self->grabbed);
                self->field_0x180 = 0;
                self->field_0x188 = 0;
            } else {
                fn_710113c304(self->Grab());
            }
        }
    }

    if (!Target(self->field_0xc8) && !Target(self->field_0xd0))
        return;  // neither holding nor held

    if (Target(self->field_0xd0))
        fn_710113def8(self->Grab(), GrabDt(self->owner, dt));

    if (UnkGrabActor_b3e82c* holder = Target(self->field_0xc8)) {
        GrabDt(holder, dt);  // the result is unused in the original
        UnkAnimationComponent* anim = self->owner->field_0x220;
        if (!self->field_0x1d8 && anim &&
            (!fn_710117b348(anim, fn_7100cfe868()) || fn_71011782ec(self->owner->field_0x220, 0xf4061ff25a2163d6)))
            fn_710113b8cc(self->Grab());
    }
}
