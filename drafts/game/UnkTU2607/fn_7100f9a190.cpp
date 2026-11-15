// From unit 2607 of tools/tu_map.py: per-frame update of the environment fade's channel 0 (a vector of four): it
// advances the channel by the frame time, blends the vector and gives it to the render manager.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::UnkVector4D;

struct UnkEnvironmentFade;

namespace {
struct UnkFadeLayout {
    UnkVector4D field_0x0;  // channel 0: where it starts
    UnkVector4D now0;       // 0x10
    UnkVector4D to0;        // 0x20
    u8 pad_0x30[0x318];
    f32 progress[29];  // 0x348
    f32 duration[29];  // 0x3bc
    bool done[29];     // 0x430
};
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");

// A vector of four faded from `from` to `to`.
inline UnkVector4D FadeLerp4(f32 t, const UnkVector4D& from, const UnkVector4D& to) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    UnkVector4D a = fn_710007b5c0(&from, 1.0f - t);
    UnkVector4D b = fn_710007b5c0(&to, t);
    return fn_710007b5ec(&a, &b);
}
}  // namespace

// The render manager's setter that takes the blended vector.
void fn_7100219ef4(UnkRenderManager* self, const f32* rect, u32 view);

void fn_7100f9a190(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = reinterpret_cast<UnkFadeLayout*>(fade);
    f32 t;
    bool finished;
    if (self->duration[0] <= 0.001f || (t = dt / self->duration[0] + self->progress[0], self->progress[0] = t,
                                        t >= 0.99f)) {
        t = 1.0f;
        self->progress[0] = 1.0f;
        finished = true;
    } else {
        finished = false;
    }
    self->done[0] = finished;
    UnkVector4D r = FadeLerp4(t, self->now0, self->to0);
    fn_7100219ef4(g_unk_7101d47300, &r.x, 0);
}
