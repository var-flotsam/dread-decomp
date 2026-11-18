// CDaivoAIComponent's unit: counts down the spit attack's cooldown, holding it while a target is set and
// restarting it from the CCharClassDaivoSpitAttack's cooldown once the hold ends.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

namespace {
struct UnkDaivoSpitObject {
    virtual const base::reflection::CClass* vf_0x0();
};
struct UnkDaivoSpitAttack : UnkDaivoSpitObject {
    u8 pad_0x8[0xe4];
    f32 field_0xec;
};
struct UnkDaivoSpitTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkDaivoSpitState {
    f32 cooldown;                     // 0x0
    bool held;                        // 0x4
    u8 pad_0x5[0xb];
    UnkDaivoSpitTarget* target;       // 0x10
    u8 pad_0x18[0x20];
    UnkDaivoSpitObject* attack;       // 0x38
};
}  // namespace

struct UnkDaivoAIComponent_4df6b8 {
    u8 pad_0x0[0x14880];
    UnkDaivoSpitState spit;  // 0x14880
};

void fn_71004df6b8(f32 dt, UnkDaivoAIComponent_4df6b8* self) {
    UnkDaivoSpitState& spit = self->spit;
    bool hasTarget = spit.target && spit.target->field_0x10;
    if (hasTarget) {
        spit.held = true;
    } else if (spit.held) {
        UnkDaivoSpitObject* object = spit.attack;
        UnkDaivoSpitAttack* attack =
            fn_710009d50c(object->vf_0x0(), fn_71004db91c()) ? static_cast<UnkDaivoSpitAttack*>(object) : nullptr;
        spit.cooldown = attack->field_0xec;
        spit.held = false;
    } else if (spit.cooldown > 0.0f) {
        spit.cooldown -= dt;
    }
}
