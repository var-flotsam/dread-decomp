// UnkTU811 (CEmmyAIComponent's unit): counts down the grab timer at +0x2a36c and, when it runs out, stops
// the owner's animation key, tells the E.M.M.I. registry, and hands one of the owner's grab entries to the
// target's grab component (fn_710113d940). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CGrabComponent.h"
#include "game/UnkActor.h"
#include "types.h"

void fn_7100dacb50(UnkEmmyRegistry_b55* registry, void* actor);
extern base::global::CStrId g_unk_71726e20b0 __attribute__((visibility("hidden")));

struct UnkGrabEntry_l54cff8 {
    u8 pad_0x0[0x88];
    UnkGrabEntry_l54cff8* next;  // 0x88
};

struct UnkGrabList_l54cff8 {
    u8 pad_0x0[0x8];
    u32 count;                    // 0x8
    UnkGrabEntry_l54cff8* first;  // 0x10
    UnkGrabEntry_l54cff8* last;   // 0x18

    UnkGrabEntry_l54cff8* At(u32 index) {
        if (index >= count)
            return nullptr;
        UnkGrabEntry_l54cff8* const* link = &first;
        for (u32 i = 0; i < count; ++i) {
            UnkGrabEntry_l54cff8* entry = *link;
            if (i == index)
                return entry;
            link = &entry->next;
        }
        return nullptr;
    }
};

struct UnkGrabHolder_l54cff8 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x50];
    UnkGrabList_l54cff8 grabs;  // 0x58
};

struct UnkGrabSlot_l54cff8 {
    u8 pad_0x0[0x10];
    UnkGrabHolder_l54cff8* holder;  // 0x10
};

struct UnkGrabOwner_l54cff8 {
    u8 pad_0x0[0x218];
    void* movement;                    // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    UnkGrabSlot_l54cff8* grab;         // 0x260
};

struct UnkEmmyAI_l54cff8 {
    u8 pad_0x0[0x30];
    UnkGrabOwner_l54cff8* owner;  // 0x30
    u8 pad_0x38[0x2a369 - 0x38];
    bool handedOver;              // 0x2a369
    u8 pad_0x2a36a[0x2];
    f32 grabTimer;                // 0x2a36c
};
static_assert(__builtin_offsetof(UnkEmmyAI_l54cff8, grabTimer) == 0x2a36c, "UnkEmmyAI_l54cff8 layout");

void fn_710054cff8(UnkEmmyAI_l54cff8* self, f32 dt) {
    if (!(self->grabTimer > 0.0f))
        return;
    self->grabTimer -= dt;
    if (!(self->grabTimer <= 0.0f))
        return;
    self->grabTimer = 0.0f;

    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117ab64(animation, false, &g_unk_71726e20b0);
    fn_7100dacb50(g_unk_71726af0a8, self);

    UnkGrabSlot_l54cff8* slot = self->owner->grab;
    if (!slot)
        return;
    UnkGrabHolder_l54cff8* holder = slot->holder;
    if (!holder)
        return;
    if (!fn_710009d50c(holder->vf_0x0(), fn_7100790104()))
        return;

    // While the owner moves in state 4 or 3, the entry before the last one; otherwise the last.
    UnkGrabList_l54cff8* grabs = &holder->grabs;
    UnkGrabEntry_l54cff8* entry;
    if (self->owner->movement &&
        (fn_71010cc38c(self->owner->movement) == 4 || fn_71010cc38c(self->owner->movement) == 3))
        entry = grabs->At(grabs->count - 2);
    else
        entry = grabs->last;

    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    if (target) {
        void* targetGrab = reinterpret_cast<UnkGrabOwner_l54cff8*>(target)->grab;
        fn_710113d940(targetGrab, reinterpret_cast<UnkKraidGrab_b57a4*>(entry), self->owner);
        self->handedOver = true;
    }
}
