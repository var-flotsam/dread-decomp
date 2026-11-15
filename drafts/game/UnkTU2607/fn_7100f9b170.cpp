// From unit 2607 of tools/tu_map.py: per-frame update of the environment fade's channel 28 (three floats): it
// advances the channel by the frame time, blends its start and end values and hands the result to the render
// manager. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::math::CVector4D;

struct UnkEnvironmentFade;

// The render manager's setter that takes the blended three floats (generated, src/lifted/).
void fn_710021929c(UnkRenderManager* self, const CVector4D* value);

namespace {
struct UnkFadeFloat {
    f32 now;
    f32 from;
    f32 to;
};

struct UnkFadeLayout {
    u8 pad_0x0[0x320];
    UnkFadeFloat f28[3];  // 0x320
    bool flag_0x344;
    u8 pad_0x345[0x3];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");
}  // namespace

void fn_7100f9b170(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = reinterpret_cast<UnkFadeLayout*>(fade);
    f32 t;
    bool finished;
    if (self->duration[28] <= 0.001f || (t = dt / self->duration[28] + self->progress[28], self->progress[28] = t,
                                         t >= 0.99f)) {
        t = 1.0f;
        self->progress[28] = 1.0f;
        finished = true;
    } else {
        finished = false;
    }
    self->done[28] = finished;
    f32 x;
    f32 y;
    f32 z;
    if (t >= 1.0f) {
        x = self->f28[0].to;
        y = self->f28[1].to;
        z = self->f28[2].to;
    } else if (t < 0.0f) {
        x = self->f28[0].from;
        y = self->f28[1].from;
        z = self->f28[2].from;
    } else {
        f32 k = 1.0f - t;
        x = self->f28[0].to * t + self->f28[0].from * k;
        y = k * self->f28[1].from + t * self->f28[1].to;
        z = k * self->f28[2].from + t * self->f28[2].to;
    }
    self->f28[0].now = x;
    self->f28[1].now = y;
    self->f28[2].now = z;
    CVector4D value = {x, y, z, 0.0f};
    fn_710021929c(g_unk_7101d47300, &value);
    self->flag_0x344 = false;
}
