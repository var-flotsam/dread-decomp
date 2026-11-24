// Constructor of a component base (shared by 25 derived constructors): runs the base
// constructor, sets the vtable, copies the default id and default-initialises the rest.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CCharClassProjectileMovement.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkComponent_0x138 {
    void** vtable;
    u8 unk_0x8[0xe8];
    base::global::CStrId field_0xf0;  // CStrId
    bool bUseCheckBeforeCast;
    u32 field_0xfc;
    bool bApplyGravityOnInitialSpeed;
    base::global::CStrId field_0x108;  // CStrId
    base::global::CStrId field_0x110;  // CStrId
    bool bDeleteAfterCollisionProcessed;
    bool bDestroyOnImpactSpeedBooster;
    u8 unk_0x11a[0xe];
    bool bIgnoreInvulnerableEntities;
    u8 unk_0x129[0x7];
    u16 field_0x130;
};

void fn_7100ab38a0(UnkComponent_0x138* self);

// CStrId's default constructor: the pool's empty string, creating the pool if needed.
static inline void InitEmptyId(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100ac14f8(UnkComponent_0x138* self) {
    fn_7100ab38a0(self);
    self->vtable = &g_unk_7101a96758[2];
    UnkStringData* id = base::global::CStrId::Empty().data;
    self->field_0xf0.data = id;
    if (&g_unk_7101d47108->empty != id)
        ++id->ref_count;
    self->bUseCheckBeforeCast = false;
    self->field_0xfc = 0;
    self->bApplyGravityOnInitialSpeed = true;
    InitEmptyId(&self->field_0x108.data);
    InitEmptyId(&self->field_0x110.data);
    self->bDeleteAfterCollisionProcessed = true;
    self->bDestroyOnImpactSpeedBooster = false;
    self->bIgnoreInvulnerableEntities = true;
    self->field_0x130 = 0;
}
