// Unit 2567 (the minimap's items): points a bouncing curve towards its on or off value. `immediately`
// jumps straight there; otherwise a change of direction starts the curve towards the new end, from the
// off value or from where it is now. Names and layouts are placeholders.
#include "base/UnkTimedCurve.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkBouncingCurve_eb7358 {
    UnkTimedCurve curve;
    f32 off_value;  // 0x50
    f32 on_value;   // 0x54
    f32 duration;   // 0x58
    u8 pad_0x5c[4];
    base::global::CRntString modifiers;  // 0x60
    u8 pad_0x60[0xd0 - 0x60 - sizeof(base::global::CRntString)];
    f32 value;     // 0xd0
    bool forward;  // 0xd4
    bool on;       // 0xd5
};
static_assert(__builtin_offsetof(UnkBouncingCurve_eb7358, curve) == 0x0, "UnkBouncingCurve_eb7358 layout");
static_assert(__builtin_offsetof(UnkBouncingCurve_eb7358, on_value) == 0x54, "UnkBouncingCurve_eb7358 layout");
static_assert(__builtin_offsetof(UnkBouncingCurve_eb7358, on) == 0xd5, "UnkBouncingCurve_eb7358 layout");

void fn_7100eb7358(void* object, u32 a, u32 b) {
    UnkBouncingCurve_eb7358* self = static_cast<UnkBouncingCurve_eb7358*>(object);
    const bool on = a & 1;
    const bool immediately = b & 1;
    f32 UnkBouncingCurve_eb7358::*end = on ? &UnkBouncingCurve_eb7358::on_value : &UnkBouncingCurve_eb7358::off_value;
    f32 target = self->*end;

    if (immediately) {
        fn_7100075f78(target, target, 0.0f, &self->curve, &g_unk_7101d58d10);
        self->value = target;
        self->on = on;
        return;
    }
    if (self->on == on)
        return;

    self->on = on;
    if (!on) {
        if (self->curve.duration != 0.0f) {
            self->forward = false;
            return;
        }
        self->forward = true;
        fn_7100075f78(self->value, target, self->duration, &self->curve, &self->modifiers);
        self->value = fn_71000763c0(0.0f, &self->curve);
        return;
    }
    f32 t = self->forward ? 0.0f : self->curve.time;
    self->forward = true;
    fn_7100075f78(self->off_value, target, self->duration, &self->curve, &self->modifiers);
    self->value = fn_71000763c0(t, &self->curve);
}
