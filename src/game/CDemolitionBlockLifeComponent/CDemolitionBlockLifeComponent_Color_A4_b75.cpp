// CDemolitionBlockLifeComponent unit (tools/tu_map.py, unit 1374): a colour interpolated over time (fn_71008c6b0c: adds
// the time, then sets the current colour to start + (target - start) * min(elapsed / duration, 1), or to the target once
// the time is up, and returns it). The component update fn_71008c69a4 steps it while it differs from its target.
// Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkColorLerp_A4_b75 {
    CVector4D current;  // 0x0
    CVector4D start;    // 0x10
    CVector4D target;   // 0x20
    f32 duration;       // 0x30
    f32 elapsed;        // 0x34
};

UnkVector4D fn_71008c6b0c(UnkColorLerp_A4_b75* self, f32 dt) {
    f32 t = self->elapsed + dt;
    self->elapsed = t;
    if (t < self->duration) {
        t = t / self->duration;
        f32 k = t > 1.0f ? 1.0f : t;
        Assign(self->current, self->start + (self->target - self->start) * k);
    } else {
        Assign(self->current, self->target);
    }
    return *static_cast<UnkVector4D*>(&self->current);
}
