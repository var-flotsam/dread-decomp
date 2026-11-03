// CItemLifeComponent unit (tools/tu_map.py, reflection hint). fn_71008e21fc: makes sure the owner's
// transform (+0x38 of the owner at +0x30) has its angles cached, then, when the owner has an FX world
// (+0x280), fires the "destroyFX" effect there (fn_710114fe8c) and releases its part
// (fn_7101152b90 finds it, fn_710115596c releases it). The name is hashed for each call. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "types.h"

// Signatures as src/game/CMissileMovement/CMissileMovement_Parts.cpp and
// src/game/CRinkaUnitComponent/CRinkaUnitComponent_b39.cpp declare them.
struct UnkWorld;
struct UnkPart;
struct UnkRinkaControl_ruc;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);
void fn_710115596c(UnkRinkaControl_ruc* self, void* handle);

struct UnkItemLifeOwner_b43a2 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1ec];
    UnkWorld* fx_world;  // 0x280
};
static_assert(__builtin_offsetof(UnkItemLifeOwner_b43a2, fx_world) == 0x280, "UnkItemLifeOwner_b43a2 layout");

struct UnkItemLifeComponent_b43a2 {
    u8 pad_0x0[0x30];
    UnkItemLifeOwner_b43a2* owner;  // 0x30
};

void fn_71008e21fc(UnkItemLifeComponent_b43a2* self) {
    self->owner->xform.GetAngle();
    if (UnkWorld* world = self->owner->fx_world) {
        fn_710114fe8c(world, fn_7100001570("destroyFX", 9), 1, 1);
        if (UnkPart* part = fn_7101152b90(world, fn_7100001570("destroyFX", 9)))
            fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(world), part);
    }
}
