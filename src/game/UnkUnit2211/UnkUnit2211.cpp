// Unit 2211 of tools/tu_map.py (0x7100c9bf00-0x7100c9c184, no reflected class): a timed screen tint
// on the render manager's layer 0. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkFadeColor {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};
void fn_7100219fe4(UnkRenderManager* self, u32 layer, const UnkFadeColor& color);

struct UnkTimedTint {
    u8 pad_0x0[0x8];
    u32 fade_out;  // 0x8
    f32 time;      // 0xc
    f32 duration;  // 0x10
    bool active;   // 0x14

    static bool IsZero(f32 v) { return v + -0.001f <= 0.0f && v + 0.001f >= 0.0f; }
    f32 Alpha() const {
        if (IsZero(duration)) return 1.0f;
        f32 alpha = time / duration;
        return fade_out ? 1.0f - alpha : alpha;
    }
};

// Advances the tint by `dt` (stopping at its duration) and sets layer 0 to red with the elapsed
// fraction as alpha (its complement when fading out; 1 for a zero duration).
void fn_7100c9c014(UnkTimedTint* self, f32 dt) {
    if (!self->active) return;
    self->time += dt;
    if (self->time > self->duration) {
        self->time = self->duration;
        self->active = false;
    }
    fn_7100219fe4(g_unk_7101d47300, 0, UnkFadeColor(1.0f, 0.0f, 0.0f, self->Alpha()));
}

