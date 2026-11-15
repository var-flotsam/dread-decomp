// From unit 2607 of tools/tu_map.py: per-frame update of the environment fade's channel 26 (a colour): it
// advances the channel by the frame time, blends the colour from its start to its end and flags the channel.
// Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::UnkVector4D;

struct UnkEnvironmentFade;

namespace {
struct UnkFadeLayout {
    u8 pad_0x0[0x2e0];
    UnkVector4D now26;   // 0x2e0
    UnkVector4D from26;  // 0x2f0
    UnkVector4D to26;    // 0x300
    u8 pad_0x310[0xc];
    bool flag_0x31c;
    u8 pad_0x31d[0x2b];
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

void fn_7100f9b038(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = reinterpret_cast<UnkFadeLayout*>(fade);
    f32 t;
    bool finished;
    if (self->duration[26] <= 0.001f || (t = dt / self->duration[26] + self->progress[26], self->progress[26] = t,
                                         t >= 0.99f)) {
        t = 1.0f;
        self->progress[26] = 1.0f;
        finished = true;
    } else {
        finished = false;
    }
    self->done[26] = finished;
    UnkVector4D r = FadeLerp4(t, self->from26, self->to26);
    fn_710007b518(&self->now26, &r);
    self->flag_0x31c = true;
}
