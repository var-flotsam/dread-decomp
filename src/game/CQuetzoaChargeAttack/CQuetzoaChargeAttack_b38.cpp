// CQuetzoaChargeAttack: when the owner's AI component is a (non-X) CQuetzoaAIComponent and the
// target is on the other side of the way the owner faces, tells the component to turn
// (fn_710064b3f4). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkQuetzoaAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkQuetzoaAI*)>(vtable[0])(this);
    }
};

struct UnkChargeAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_710064b3f4(UnkQuetzoaAI* ai);

static inline f32 Sign(f32 x) { return (f32)((x > 0.0f) - (x < 0.0f)); }

void fn_7100644d80(UnkChargeAttack* self) {
    UnkQuetzoaAI* ai = static_cast<UnkQuetzoaAI*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_7100649630()))
        ai = nullptr;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!ai || !target)
        return;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    f32 facing = Sign(m.m[2]);
    f32 side = Sign(target->field_0x44 - self->owner->field_0x44);
    if (facing == side)
        return;
    if (fn_710009d50c(ai->vf_0x0(), fn_710063efe4()))
        return;
    fn_710064b3f4(ai);
}
