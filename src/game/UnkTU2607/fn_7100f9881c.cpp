// From unit 2607 of tools/tu_map.py: starts the environment fade's channel 15 (a 2D value), after setting a flag.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkEnvironmentFade;

namespace {
struct UnkFadeLayout_f9881c {
    u8 pad_0x0[0x228];
    CVector2D now15;   // 0x228
    CVector2D from15;  // 0x230
    CVector2D to15;    // 0x238
    u8 pad_0x240[0x30];
    bool flag_0x270;
    u8 pad_0x271[0xd7];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout_f9881c, progress) == 0x348, "UnkFadeLayout_f9881c placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout_f9881c, done) == 0x430, "UnkFadeLayout_f9881c placeholder layout");
}  // namespace

void fn_7100f9881c(f32 time, UnkEnvironmentFade* self, const CVector2D* to) {
    UnkFadeLayout_f9881c* s = reinterpret_cast<UnkFadeLayout_f9881c*>(self);
    s->done[15] = false;
    s->duration[15] = time;
    bool lo_ok = time - 0.001f <= 0.0f;
    bool hi_ok = time + 0.001f >= 0.0f;
    s->progress[15] = hi_ok && lo_ok ? 1.0f : 0.0f;
    s->flag_0x270 = true;
    s->from15.x = s->now15.x;
    s->from15.y = s->now15.y;
    f32 x = to->x;
    f32 y = to->y;
    s->to15.x = x;
    s->to15.y = y;
}
