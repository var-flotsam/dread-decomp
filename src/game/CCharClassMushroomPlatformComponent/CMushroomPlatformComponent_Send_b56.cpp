// CCharClassMushroomPlatformComponent's unit (tools/tu_map.py): fn_710088b1f0 sends the owner's
// (+0x30) sub-object at +0x220 a fixed hash with mode 0 (as UnkSpitSend,
// src/game_s2/UnkSpitSend.h) unless fn_710117ae38 says yes for it or for another hash, then gives
// the owner's weak target (fn_71002b3b84) the id 0xb43f302c7cd8f802 (fn_71009c0f1c).
// fn_710088b8d0 (same unit) unhooks the link at +0x4e8 from the target of its weak handle (+0x500)
// and drops the handle; when the owner has the sub-object, it first sends it a fixed hash the same
// way and afterwards hooks the link up to it (fn_7100316018, fn_710117b3e8), as the attack track
// links in src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp do; then copies the
// word at +0x3f4 to +0x3f0. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU372.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeakTarget;
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);

struct UnkMushroomOwner_b56 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkMushroomPlatform_b56 {
    u8 pad_0x0[0x30];
    union {
        UnkMushroomOwner_b56* owner;
        UnkWeakTarget* weak_owner;
    };
};

void fn_710088b1f0(UnkMushroomPlatform_b56* self) {
    if (UnkMushroomOwner_b56* owner = self->owner) {
        if (UnkAnimationComponent* target = owner->field_0x220) {
            if (!fn_710117ae38(target, 0xfc63a4491a18aae8, false) &&
                !fn_710117ae38(target, 0xe2ea001660c8ac66, false))
                UnkSpitSend(target, 0xe2ea001660c8ac66, 0xf4061ff25a2163d6);
        }
    }
    if (void* target = fn_71002b3b84(self->weak_owner))
        fn_71009c0f1c(target, 0xb43f302c7cd8f802);
}

struct UnkMushroomLinkOwner_b56 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
};

struct UnkMushroomLinked_b56 {
    u8 pad_0x0[0x30];
    UnkMushroomLinkOwner_b56* owner;
    u8 pad_0x38[0x3f0 - 0x38];
    u32 field_0x3f0;
    u32 field_0x3f4;
    u8 pad_0x3f8[0x4e8 - 0x3f8];
    u8 link[0x18];
    UnkWeakHandle* handle;
};

static inline __attribute__((always_inline)) void Unhook_b56(UnkMushroomLinked_b56* self) {
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
}

void fn_710088b8d0(UnkMushroomLinked_b56* self) {
    UnkWeakTarget* track;
    if (self->owner && (track = self->owner->field_0x220)) {
        UnkSpitSend(track, 0x5c812431a6579adc, 0xf4061ff25a2163d6);
        void* link = self->link;
        Unhook_b56(self);
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    } else {
        Unhook_b56(self);
    }
    self->field_0x3f0 = self->field_0x3f4;
}
