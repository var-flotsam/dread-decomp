// CPullableGrapplePointComponent's unit (tools/tu_map.py, the name from reflection): the pull has
// finished. With a script, counts the use (+0x44 of the entity its slot-0 getter's +8 names) and calls
// "OnGrapplePullFinished" with the flag and the entity's name ("" when unset); signals the actor's
// +0x280 control with a fixed id; then notifies the deferred listener set g_unk_7172724df8 with this
// component. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CFXComponent.h"
#include "game/CPullableGrapplePointComponent.h"
#include "types.h"

struct UnkScriptCaller;
// Signature as src/game/CGrapplePointComponent/CGrapplePointComponent_b52.cpp declares it.
struct UnkRinkaControl_ruc;
// Signature as src/game/CRinkaUnitComponent/CRinkaUnitComponent_b39.cpp declares it.

extern char g_unk_7172724df8[];

struct UnkClone_fn_710082d0ac;
using UnkPullListeners_b53 = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkPullListeners_b53*, const u64*);

struct UnkPullEntity_b53 {
    u8 unk_0x0[0x10];
    const char* name;  // 0x10
    u8 unk_0x18[0x2c];
    union {
        u32 uses;  // 0x44 (no type-based alias info: the original reloads after the count)
    };
};

struct UnkPullOwner_b53 {
    u8 unk_0x0[0x8];
    UnkPullEntity_b53* entity;
};

struct UnkPullActor_b53 {
    u8 unk_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
    u8 unk_0x1e8[0x98];
    UnkRinkaControl_ruc* control;  // 0x280
};

struct UnkPullableGrapplePointComponent_b53 {
    virtual UnkPullOwner_b53* vf_0x0();
    u8 unk_0x8[0x28];
    UnkPullActor_b53* actor;  // 0x30
};

void fn_710082ccec(UnkPullableGrapplePointComponent_b53* self, void*, bool flag) {
    if (UnkScriptCaller* script = self->actor->script) {
        s32 f = flag;
        UnkPullOwner_b53* owner = self->vf_0x0();
        owner->entity->uses++;
        const char* name = owner->entity->name;
        fn_7100305088(script, "OnGrapplePullFinished", "bs", f, name ? name : "");
    }
    if (UnkRinkaControl_ruc* control = self->actor->control)
        fn_710114ffb8(control, 0x9cd5426ffb8d68a5ull, false, false);
    u64 arg = reinterpret_cast<u64>(self);
    base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(reinterpret_cast<UnkPullListeners_b53*>(g_unk_7172724df8), &arg);
}
