// CWeightActivablePropComponent's unit (tools/tu_map.py): the same code as fn_71008e21fc
// (src/game/CItemLifeComponent/CItemLifeComponent_b43.cpp): makes sure the owner's transform has its
// angles cached, then fires the "destroyFX" effect in the owner's FX world and releases its part.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

// Signatures as src/game/CMissileMovement/CMissileMovement_Parts.cpp and
// src/game/CRinkaUnitComponent/CRinkaUnitComponent_b39.cpp declare them.
struct UnkWorld;
struct UnkPart;
struct UnkRinkaControl_ruc;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);

struct UnkWeightPropOwner_b43a3 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1ec];
    UnkWorld* fx_world;  // 0x280
};
static_assert(__builtin_offsetof(UnkWeightPropOwner_b43a3, fx_world) == 0x280, "UnkWeightPropOwner_b43a3 layout");

struct UnkWeightPropComponent_b43a3 {
    u8 pad_0x0[0x30];
    UnkWeightPropOwner_b43a3* owner;  // 0x30
};

void fn_7100aca378(UnkWeightPropComponent_b43a3* self) {
    self->owner->xform.GetAngle();
    if (UnkWorld* world = self->owner->fx_world) {
        fn_710114fe8c(world, fn_7100001570("destroyFX", 9), 1, 1);
        if (UnkPart* part = fn_7101152b90(world, fn_7100001570("destroyFX", 9)))
            fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(world), part);
    }
}
