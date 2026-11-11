// CBigFistAIComponent's unit (tools/tu_map.py): fn_71003e819c runs the base update (fn_71011088ec), then
// marks the AI (+0x14a28) when the owner's current entry (+0x278, its +0x1a0) is not a CBigFistAttack;
// otherwise clears the mark and, while the animation (+0x220) runs "attack_init" (a guarded static id)
// and track 0xf4061ff25a2163d6 is under 30, flags the attack (+0x58) and sets that track to 30.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkStruct_710117af10;
struct UnkCls_710117af10;

struct UnkBigFistAttack2_ovb0078 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x50];
    bool field_0x58;
};

struct UnkBigFistSlot2_ovb0078 {
    u8 pad_0x0[0x1a0];
    UnkBigFistAttack2_ovb0078* field_0x1a0;
};

struct UnkBigFistOwner2_ovb0078 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x50];
    UnkBigFistSlot2_ovb0078* field_0x278;
};

struct UnkBigFistAI2_ovb0078 {
    u8 pad_0x0[0x30];
    UnkBigFistOwner2_ovb0078* owner;
    u8 pad_0x38[0x14a28 - 0x38];
    bool field_0x14a28;
};
void fn_71011088ec(UnkBigFistAI2_ovb0078* self);

// The id "attack_init", built on first use.
struct UnkGuardedId_ovb0078 {
    u8 id[8];
    u64 guard;
};
extern UnkGuardedId_ovb0078 g_unk_71726c1108 __attribute__((visibility("hidden")));

void fn_71003e819c(UnkBigFistAI2_ovb0078* self) {
    fn_71011088ec(self);
    UnkBigFistAttack2_ovb0078* attack = self->owner->field_0x278->field_0x1a0;
    if (!attack || !fn_710009d50c(attack->vf_0x0(), fn_71003e05e0())) {
        self->field_0x14a28 = true;
        return;
    }
    self->field_0x14a28 = false;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726c1108.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726c1108.guard)) {
            new (g_unk_71726c1108.id) CStrId("attack_init", true);
            fn_7100000250(fn_710000051c, g_unk_71726c1108.id, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71726c1108.guard);
        }
    }
    if (!fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(self->owner->field_0x220),
                       reinterpret_cast<UnkCls_710117af10**>(g_unk_71726c1108.id), false))
        return;
    const u64 key = 0xf4061ff25a2163d6;
    if (!(fn_7101177ea0(self->owner->field_0x220, key) < 30.0f))
        return;
    attack->field_0x58 = true;
    fn_7101178010(self->owner->field_0x220, key, false, 30.0f);
}
