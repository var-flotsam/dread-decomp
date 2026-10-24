// When the owner faces the target (the sign of its direction's x equals the sign of the x offset to
// the target) and the target's object at +0x1b8 fails a test, hands the owner's component at +0x240
// to fn_71004554d8 if its class is the commander AI component.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::CClass;

struct UnkComponent {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkComponent*)>(vtable[0])(this); }
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
};

bool fn_7101161088(void* object);
const CClass* fn_710044e708();  // CChozoCommanderAIComponent::refl_GetClass
void fn_71004554d8(UnkComponent* component);

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

void fn_7100428010(UnkSelf* self) {
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (target) {
        f32 facing = Sign(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38)).m[2]);
        f32 side = Sign(target->field_0x44 - self->field_0x8->field_0x44);
        if (facing == side && target->field_0x1b8 && !fn_7101161088(target->field_0x1b8)) {
            UnkComponent* component = static_cast<UnkComponent*>(self->field_0x8->field_0x240);
            if (component && fn_710009d50c(component->vf_0x0(), fn_710044e708()))
                fn_71004554d8(component);
        }
    }
}
