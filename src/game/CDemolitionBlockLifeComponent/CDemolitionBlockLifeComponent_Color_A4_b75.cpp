// CDemolitionBlockLifeComponent unit (tools/tu_map.py, unit 1374): a colour interpolated over time (fn_71008c6b0c: adds
// the time, then sets the current colour to start + (target - start) * min(elapsed / duration, 1), or to the target once
// the time is up, and returns it). The component update fn_71008c69a4 steps it while it differs from its target.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

UnkVector4D fn_710007b618(const CVector4D* a, const CVector4D* b);  // a - b
UnkVector4D fn_710007b5c0(const CVector4D* a, f32 s);               // a * s
UnkVector4D fn_710007b5ec(const CVector4D* a, const CVector4D* b);  // a + b

struct UnkColorLerp_A4_b75 {
    CVector4D current;  // 0x0
    CVector4D start;    // 0x10
    CVector4D target;   // 0x20
    f32 duration;       // 0x30
    f32 elapsed;        // 0x34
};

static inline __attribute__((always_inline)) UnkVector4D Sub_A4_b75(const CVector4D& a, const CVector4D& b) {
    return fn_710007b618(&a, &b);
}
static inline __attribute__((always_inline)) UnkVector4D Mul_A4_b75(const CVector4D& a, f32 s) {
    return fn_710007b5c0(&a, s);
}
static inline __attribute__((always_inline)) UnkVector4D Add_A4_b75(const CVector4D& a, const CVector4D& b) {
    return fn_710007b5ec(&a, &b);
}
static inline __attribute__((always_inline)) void Assign_A4_b75(CVector4D* self, const CVector4D& v) {
    fn_710007b518(self, &v);
}

UnkVector4D fn_71008c6b0c(UnkColorLerp_A4_b75* self, f32 dt) {
    f32 t = self->elapsed + dt;
    self->elapsed = t;
    if (t < self->duration) {
        t = t / self->duration;
        f32 k = t > 1.0f ? 1.0f : t;
        Assign_A4_b75(&self->current, Add_A4_b75(self->start, Mul_A4_b75(Sub_A4_b75(self->target, self->start), k)));
    } else {
        fn_710007b518(&self->current, &self->target);
    }
    return *static_cast<UnkVector4D*>(&self->current);
}
