// CWaterPlatformUsableComponent unit (tools/tu_map.py, unit 1608): the update. Unless already in use, the
// linked actor's "SMARTOBJECT" component (fn_710033dc00) must be a platform that holds (fn_7100a04768), or state 1
// is requested from the component's state machine at +0x100 ("none" 8). A countdown at +0x5b8 restarts the curve
// at +0x568 when it runs out, then fn_7100aa5e84 runs with whether to start. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/UnkTimedCurve.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CSmartObjectComponent.h"
#include "types.h"

struct UnkRefl_fn_710033dc00;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710033dc00>(void*);

struct UnkPlatformSm_A5_b72;

struct UnkWaterPlatform_A5_b73;
bool fn_7100aa5980(UnkWaterPlatform_A5_b73* self);
void fn_7100aa5e84(UnkWaterPlatform_A5_b73* self, bool start, f32 dt);

struct UnkWaterPlatformLink_A5_b73 {
    u8 pad_0x0[0x10];
    void* actor;
};

struct UnkWaterPlatform_A5_b73 {
    u8 pad_0x0[0xe0];
    UnkWaterPlatformLink_A5_b73* link;
    u8 pad_0xe8[0x100 - 0xe8];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x1b0[0x564 - 0x1b0];
    f32 target;
    UnkTimedCurve curve;
    f32 countdown;
    u8 pad_0x5bc[0x5c0 - 0x5bc];
    f32 field_0x5c0;
    bool in_use;
    bool field_0x5c5;
    bool field_0x5c6;
};
static_assert(__builtin_offsetof(UnkWaterPlatform_A5_b73, countdown) == 0x5b8, "layout");

void fn_7100aa6558(UnkWaterPlatform_A5_b73* self, f32 dt) {
    if (!self->in_use) {
        void* actor = self->link ? self->link->actor : nullptr;
        auto* object = reinterpret_cast<UnkPlatformSm_A5_b72*>(
            base::reflection::refl_FindTyped<UnkRefl_fn_710033dc00>(actor));
        if (!object || !fn_7100a04768(object)) {
            RequestState_A5_b73<15, 8>(&self->machine, 1);
            return;
        }
    }
    if (self->countdown > 0.0f) {
        f32 t = self->countdown - dt;
        self->countdown = t;
        if (t <= 0.0f) {
            f32 target = self->target;
            self->countdown = 0.0f;
            UnkTimedCurve* curve = &self->curve;
            asm("" : "+r"(curve));  // the original computes the curve's address before the string
            base::global::CRntString modifiers("");
            fn_7100075f78(target, 1.0f, 0.25f, curve, &modifiers);
        }
    }
    bool start;
    if (fn_7100aa5980(self) && self->countdown == 0.0f)
        start = true;
    else if (self->field_0x5c0 > 0.0f || (self->in_use && !self->field_0x5c5))
        start = false;
    else
        goto done;
    fn_7100aa5e84(self, start, dt);
done:
    self->field_0x5c6 = false;
}
