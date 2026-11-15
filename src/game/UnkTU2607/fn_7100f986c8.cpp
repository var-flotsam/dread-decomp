// From unit 2607 of tools/tu_map.py: starts the environment fade's channel 12 (a 3D value).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEnvironmentFade;

namespace {
struct UnkFadeLayout_f986c8 {
    u8 pad_0x0[0x1ac];
    CVector2D from12xy;  // 0x1ac
    f32 from12z;         // 0x1b4
    CVector3D to12;    // 0x1b8
    CVector2D now12xy;   // 0x1c4
    f32 now12z;          // 0x1cc
    u8 pad_0x1d0[0x178];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout_f986c8, progress) == 0x348, "UnkFadeLayout_f986c8 placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout_f986c8, done) == 0x430, "UnkFadeLayout_f986c8 placeholder layout");
}  // namespace

void fn_7100f986c8(f32 time, UnkEnvironmentFade* self, const CVector3D* to) {
    UnkFadeLayout_f986c8* s = reinterpret_cast<UnkFadeLayout_f986c8*>(self);
    s->done[12] = false;
    s->duration[12] = time;
    bool lo_ok = time - 0.001f <= 0.0f;
    bool hi_ok = time + 0.001f >= 0.0f;
    s->progress[12] = hi_ok && lo_ok ? 1.0f : 0.0f;
    s->from12xy = s->now12xy;
    s->from12z = s->now12z;
    s->to12.x = to->x;
    s->to12.y = to->y;
    s->to12.z = to->z;
}
