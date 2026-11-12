// Unit CBigFistAIComponent: picks a float result (1.5 or -1) from the owner's attack state.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "types.h"

struct UnkAttackObj {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x08[0x50];
    u8 field_0x58;
};

struct UnkAttackOwner {
    u8 pad_0x00[0x218];
    void* pMovement;  // 0x218
    u8 pad_0x220[0x58];
    void* pAttackComp;  // 0x278
};
struct UnkAttackComp {
    u8 pad_0x00[0x1a0];
    UnkAttackObj* pAttack;  // 0x1a0
};

struct UnkBigFist {
    u8 pad_0x00[0x30];
    UnkAttackOwner* pOwner;  // 0x30
    u8 pad_0x38[0x14a28 - 0x38];
    u8 field_0x14a28;
};

f32 fn_71003e82f0(UnkBigFist* self) {
    void* comp = self->pOwner->pAttackComp;
    if (comp && fn_7101136308(comp)) {
        UnkAttackObj* attack = static_cast<UnkAttackComp*>(self->pOwner->pAttackComp)->pAttack;
        if (attack && fn_710009d50c(attack->vf_0x0(), fn_71003e05e0()) && attack->field_0x58)
            return 1.5f;
        return -1.0f;
    }
    if (!self->field_0x14a28)
        return -1.0f;
    if (self->pOwner->pMovement) {
        fn_71010cab0c(self->pOwner->pMovement);
        f32 r = 1.5f;
#if defined(__aarch64__)
        // keeps the 1.5 constant from being hoisted above the call, as the original does
        asm("" : "+w"(r));
#endif
        return r;
    }
    return 1.5f;
}
