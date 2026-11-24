// In the CCharClassBigkranXSpitAttack unit: a constructor (vtable 0x7101a312d0) over the attack base
// constructor fn_71003641fc: five floats at +0xec set to -1, an empty array at +0x148 (step 10,
// flags 0xf, the default allocator) and an empty string id at +0x170. Layout placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassAttack.h"
#include "game/CCharClassBigkranXSpitAttack.h"
#include "types.h"

// Declared as src/game/CCharClassAttack/fn_71003641fc.cpp defines it.
struct UnkStruct_71003641fc;

struct UnkBigkranXSpitAttackDef {
    void** vtable;
    u8 pad_0x8[0xe4];
    f32 fTimeAttacking;
    f32 fTargetBehindMaxTime;
    f32 fTargetAheadMaxDistance;
    f32 fTargetBehindMaxDistance;
    f32 fTimeOutOfFrustumToAbortAttack;
    u8 pad_0x100[0x48];
    base::global::CRntVector<void*> field_0x148;
    base::global::UnkStringData* field_0x170;
};

void fn_71003f0aac(UnkBigkranXSpitAttackDef* self) {
    fn_71003641fc(reinterpret_cast<UnkStruct_71003641fc*>(self));
    self->vtable = g_unk_7101a312d0 + 2;
    self->fTimeAttacking = -1.0f;
    self->fTargetBehindMaxTime = -1.0f;
    self->fTargetAheadMaxDistance = -1.0f;
    self->fTargetBehindMaxDistance = -1.0f;
    self->fTimeOutOfFrustumToAbortAttack = -1.0f;
    new (&self->field_0x148) base::global::CRntVector<void*>(0xf);
    self->field_0x170 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x170 = &g_unk_7101d47108->empty;
}
