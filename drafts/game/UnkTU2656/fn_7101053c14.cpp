// From unit 2656 of tools/tu_map.py (0x7101053650-0x7101053e24): arms the screen fade
// (src/game/UnkScreenFade.cpp) with a colour, or, with no delay, starts it at once.
// Names and layouts are placeholders.
#include "game/CBossAIComponent.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU286.h"
#include "game/UnkTU315.h"
#include "types.h"

struct UnkFadeColor {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    u8 flag;  // +0xb8
};

struct UnkFadeParams {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
    f32 duration;
    f32 delay;
};

struct UnkScreenFade {
    u8* target;  // its node is at +0x18
    bool enabled;
    bool armed;  // +0x9
    u8 unk_0xa[0x2];
    UnkFadeParams params;  // +0xc
    f32 elapsed;           // +0x24
    u32 field_0x28;
};

void fn_71001c9048(UnkGlobal_710b372a98* self);

static inline f32 NonNegative(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

// NON_MATCHING(minor): only the order of the three argument copies into callee-saved registers differs.
void fn_7101053c14(void* fx, const base::math::CVector3D* color, f32 delay, f32 duration, f32 alpha) {
    UnkScreenFade* self = static_cast<UnkScreenFade*>(fx);
    if (fn_7101198688(g_unk_7101d47330))
        return;
    if (!(delay > 0.0f)) {
        if (g_unk_71726bc570->flag) {
            self->armed = false;
            self->params = UnkFadeParams();
            self->elapsed = 0.0f;
        }
        fn_71001c9048(g_unk_710b372a98);
        fn_7100219fe4(g_unk_7101d47300, 0, UnkFadeColor(1.0f, 0.0f, 0.0f, 1.0f));
        self->elapsed = 0.0f;
        f32 cr = color->x;
        f32 cg = color->y;
        f32 cb = color->z;
        f32 r = NonNegative(cr);
        f32 g = NonNegative(cg);
        f32 b = NonNegative(cb);
        f32 a = NonNegative(alpha);
        UnkFadeColor to(r, g, b, a);
        UnkFadeColor from(to.r, to.g, to.b, 0.0f);
        fn_7100209968(self->target + 0x18, duration, &to, &from);
        self->field_0x28 = 0;
        return;
    }
    self->params.duration = 0.0f;
    self->params.delay = 0.0f;
    self->elapsed = 0.0f;
    self->params.b = 0.0f;
    self->params.a = 0.0f;
    self->params.r = 0.0f;
    self->params.g = 0.0f;
    self->armed = true;
    self->params.duration = duration;
    self->params.r = color->x;
    self->params.g = color->y;
    self->params.b = color->z;
    self->params.a = alpha;
    self->params.delay = delay;
}
