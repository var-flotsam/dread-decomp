// From unit 2607 of tools/tu_map.py: per-frame update of the environment fade's channel 5 (two sets of four
// floats, interleaved with their end values): it advances the channel by the frame time, blends the floats
// and hands the two vectors to the render manager. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::math::CVector4D;

struct UnkEnvironmentFade;

// The render manager's setters that take the two blended vectors (generated, src/lifted/).
void fn_71002192b0(UnkRenderManager* self, const CVector4D* value);
void fn_71002192bc(UnkRenderManager* self, const CVector4D* value);

namespace {
struct UnkFadePair {
    f32 from;
    f32 to;
};

struct UnkFadeLayout {
    u8 pad_0x0[0xa4];
    UnkFadePair field_0xa4[8];  // 0xa4: channel 5
    u8 pad_0xe4[0x264];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");
}  // namespace

void fn_7100f9a560(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = reinterpret_cast<UnkFadeLayout*>(fade);
    f32 t;
    bool finished;
    if (self->duration[5] <= 0.001f || (t = dt / self->duration[5] + self->progress[5], self->progress[5] = t,
                                        t >= 0.99f)) {
        t = 1.0f;
        self->progress[5] = 1.0f;
        finished = true;
    } else {
        finished = false;
    }
    self->done[5] = finished;
    UnkFadePair* p = self->field_0xa4;
    f32 to0 = p[0].to;
    f32 x0, x1, x2, y0, y1, y2;
    if (t >= 1.0f) {
        x0 = to0;
        x1 = p[1].to;
        x2 = p[2].to;
        y0 = p[4].to;
        y1 = p[5].to;
        y2 = p[6].to;
    } else if (t < 0.0f) {
        x0 = p[0].from;
        x1 = p[1].from;
        x2 = p[2].from;
        y0 = p[4].from;
        y1 = p[5].from;
        y2 = p[6].from;
    } else {
        f32 k = 1.0f - t;
        x0 = to0 * t + p[0].from * k;
        x1 = k * p[1].from + t * p[1].to;
        x2 = k * p[2].from + t * p[2].to;
        y0 = k * p[4].from + t * p[4].to;
        y1 = k * p[5].from + t * p[5].to;
        y2 = k * p[6].from + t * p[6].to;
    }
    CVector4D first = {x0, x1, x2, p[3].to};
    fn_71002192b0(g_unk_7101d47300, &first);
    CVector4D second = {y0, y1, y2, p[7].to};
    fn_71002192bc(g_unk_7101d47300, &second);
}
