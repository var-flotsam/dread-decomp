// behaviortree::CEmmySearchingAction's unit (tools/tu_map.py unit 805): the look-around at a search stop.
// In movement mode 4 it plays the movement's pending target animation when it may; in mode 1 it plays a
// look animation chosen by the movement; when nothing plays, the search resumes (fn_7100519b48). `played`
// and `fromTarget` say what happened. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU1444.h"
#include "game/UnkTU376.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkEmmySearchingAction_b0068;
struct UnkEmmySearchingAction_b63a3;
void fn_7100519b48(UnkEmmySearchingAction_b63a3* self);
void fn_710051d568();

// The movement's pending target: its animation id at +0x38 and whether it is to be played (+0x4c).
struct UnkEmmyTarget_l51c018 {
    u8 pad_0x0[0x38];
    base::global::UnkStringData* name;  // 0x38
    u8 pad_0x40[0xc];
    bool play;  // 0x4c
    u8 pad_0x4d[0xb3];

    UnkEmmyTarget_l51c018() { fn_71002b57bc(Object()); }
    ~UnkEmmyTarget_l51c018() { fn_71002b6094(Object()); }
    void Assign(const UnkObject_0x100* other) { fn_71002b5c90(Object(), other); }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
};

struct UnkEmmyMovement_l51c018 {
    void** vtable;
    u8 pad_0x8[0x9dc];
    u8 field_0x9e4[0x4];
    u8 pad_0x9e8[0xdb8];
    u8 event[0xd0];          // 0x17a0
    UnkObject_0x100 target;  // 0x1870
    bool hasTarget;          // 0x1970
    u8 pad_0x1971[0x32];
    bool locked;  // 0x19a3

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEmmyMovement_l51c018*)>(vtable[0])(this);
    }
};
static_assert(__builtin_offsetof(UnkEmmyMovement_l51c018, locked) == 0x19a3, "layout");

s32 fn_71005717d4(UnkEmmyMovement_l51c018* self, void* arg);

struct UnkEmmyActor_l51c018 {
    u8 pad_0x0[0x1f0];
    UnkEmmyMovement_l51c018* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    void* animation;  // 0x220
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* ai;  // 0x240
};

// A listener on an animation: the callback with its context, and the weak handle of what it listens to.
struct UnkEmmyAnimLink_l51c018 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
    UnkWeakHandle* handle;  // 0x18

    // Stops listening to the animation the handle points at, and drops the handle.
    void Detach() {
        if (UnkWeakHandle* h = handle) {
            if (UnkWeakTarget* target = h->target)
                fn_710117b3f0(target, this);
            ResetRef(&handle);
        }
    }
    // Listens to `animation` instead.
    void Attach(UnkWeakTarget* animation) {
        Detach();
        if (animation) {
            fn_7100316018(animation, &handle);
            fn_710117b3e8(handle ? handle->target : nullptr, this);
        }
    }
};

struct UnkEmmySearchingAction_l51c018 {
    u8 pad_0x0[0x18];
    UnkEmmyActor_l51c018* actor;  // 0x18
    u8 pad_0x20[0x20];
    UnkEmmyAnimLink_l51c018 link;  // 0x40
    u8 pad_0x60[0x12ad6];
    bool targetPlayed;  // 0x12b36
    u8 pad_0x12b37;
    u64 targetAnimation;  // 0x12b38
};
static_assert(__builtin_offsetof(UnkEmmySearchingAction_l51c018, targetAnimation) == 0x12b38, "layout");

// Copies the movement's pending target into `target`; true when there is one to play.
static inline bool TakeTarget_l51c018(UnkEmmyMovement_l51c018* movement, UnkEmmyTarget_l51c018* target) {
    if (!movement->hasTarget)
        return false;
    target->Assign(&movement->target);
    return movement->hasTarget && target->play;
}

void fn_710051c018(UnkEmmySearchingAction_b0068* action, bool* fromTarget, bool* played) {
    auto* self = reinterpret_cast<UnkEmmySearchingAction_l51c018*>(action);
    // The requests' arguments, each with a slot of its own.
    base::reflection::UnkReflTypeIdArg modeA;
    base::reflection::UnkReflTypeIdArg maskA;
    base::reflection::UnkReflTypeIdArg modeC;
    base::reflection::UnkReflTypeIdArg maskC;
    base::reflection::UnkReflTypeIdArg modeB;
    base::reflection::UnkReflTypeIdArg maskB;
    *played = false;
    *fromTarget = false;
    base::reflection::UnkReflObject* ai = self->actor->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        return;
    void* animation = self->actor->animation;
    UnkEmmyMovement_l51c018* movement = self->actor->movement;
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_710057134c()))
        movement = nullptr;

    if (fn_71010cc3a4(movement) == 4) {
        UnkEmmyTarget_l51c018 target;
        if (movement->locked || fn_710117b348(static_cast<UnkAnimationComponent*>(animation), fn_7100c0f988()) ||
            !TakeTarget_l51c018(movement, &target)) {
            fn_7100974798(reinterpret_cast<UnkUnit1444_Event*>(movement->event), 1);
        } else {
            self->targetPlayed = true;
            self->targetAnimation = target.name->Key();
            self->link.context = self;
            self->link.callback = fn_710051d568;
            self->link.Attach(static_cast<UnkWeakTarget*>(animation));
            *played = true;
            *fromTarget = true;
            return;
        }
    }

    if (fn_71010cc3a4(movement) == 1) {
        bool sent;
        if (fn_7100979ef0(movement)) {
            modeA.id = 0;
            maskA.id = UnkAll_SpitSend().value;
            sent = fn_7101176690(animation, 0x33723105a0f4830b, 0xf4061ff25a2163d6, &modeA, &g_unk_710164b958,
                                 &g_unk_7172769740, &maskA);
        } else if (fn_71005717d4(movement, movement->field_0x9e4)) {
            modeB.id = 0;
            maskB.id = UnkAll_SpitSend().value;
            sent = fn_7101176690(animation, 0x0318db99da3da690, 0xf4061ff25a2163d6, &modeB, &g_unk_710164b958,
                                 &g_unk_7172769740, &maskB);
        } else {
            modeC.id = 0;
            maskC.id = UnkAll_SpitSend().value;
            sent = fn_7101176690(animation, 0x7a4ea211f0e3ce50, 0xf4061ff25a2163d6, &modeC, &g_unk_710164b958,
                                 &g_unk_7172769740, &maskC);
        }
        if (sent) {
            self->link.context = self;
            self->link.callback = fn_710051d568;
            self->link.Attach(static_cast<UnkWeakTarget*>(animation));
            *played = true;
            return;
        }
    }
    fn_7100519b48(reinterpret_cast<UnkEmmySearchingAction_b63a3*>(self));
}
