// From unit 2656 of tools/tu_map.py (0x7101053650-0x7101053e24, no reflected class): the screen
// fade the game manager embeds at +0x1040 (game/UnkGameManager.h calls it an effect player). A fade
// is armed with a colour, a delay and a duration; once the delay has run out its fade node is
// started. Names and layouts are placeholders.
// Holds fn_7101053aa4 (the per-frame update).
#include "game/UnkGameManager.h"
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkFadeColor {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

// The object whose node at +0x18 shows the fade, and whether it is hidden for the mode holder.
struct UnkFadeTarget {
    u8 unk_0x0[0x18];
    u8 node[0x138];
    // A union member (no type-based alias info): the original reloads the target after storing it.
    union {
        bool hidden;  // +0x150
    };
};

// What a fade is armed with.
struct UnkFadeParams {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
    f32 duration;
    f32 delay;
};

struct UnkScreenFade {
    UnkFadeTarget* target;
    bool enabled;  // +0x8
    bool armed;    // +0x9
    u8 unk_0xa[0x2];
    UnkFadeParams params;  // +0xc
    f32 elapsed;           // +0x24
    u32 field_0x28;
};
static_assert(__builtin_offsetof(UnkScreenFade, field_0x28) == 0x28, "UnkScreenFade placeholder layout");

// The game's mode holder (UnkGameManager::field_0x498): vf_0x68 says whether it is set up.
struct UnkModeHolder {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual bool vf_0x68();
};

void fn_71001c9048(UnkGlobal_710b372a98* self);
void fn_7100219fe4(UnkRenderManager* self, u32 layer, const UnkFadeColor& color);
void fn_7100209968(void* node, f32 duration, const UnkFadeColor* to, const UnkFadeColor* from);
bool fn_7100209960(void* node);
void fn_7100209fec(void* node, bool visible);

static inline f32 NonNegative(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

// Advances an armed fade by `dt`: once its delay has passed, the render manager's layer 0 is reset
// to opaque red (as the original passes it) and the node fades from the colour (alpha 0) to the
// colour with its alpha over the duration, and the fade is disarmed. While enabled and its node is
// active, the target is hidden while the mode holder is set up and shown otherwise.
// NON_MATCHING(minor): register allocation and scheduling only: the colour's blue and the zero it
// is clamped against are in swapped s registers (s2/s3), and the address of the target colour
// (x1) is formed after the stores instead of before the clamps.
void fn_7101053aa4(f32 dt, UnkScreenFade* self) {
    if (self->armed) {
        UnkFadeParams& params = self->params;
        self->elapsed += dt;
        if (self->elapsed > params.delay) {
            f32 a = params.a;
            f32 duration = params.duration;
            fn_71001c9048(g_unk_710b372a98);
            fn_7100219fe4(g_unk_7101d47300, 0, UnkFadeColor(1.0f, 0.0f, 0.0f, 1.0f));
            self->elapsed = 0.0f;
            UnkFadeColor to(NonNegative(params.r), NonNegative(params.g), NonNegative(params.b), NonNegative(a));
            UnkFadeColor from(to.r, to.g, to.b, 0.0f);
            fn_7100209968(self->target->node, duration, &to, &from);
            self->field_0x28 = 0;
            self->armed = false;
            params = UnkFadeParams();
        }
    }
    if (self->enabled && fn_7100209960(self->target->node)) {
        UnkModeHolder* holder = static_cast<UnkModeHolder*>(g_unk_7101d47310->field_0x498);
        if (holder && holder->vf_0x68()) {
            if (self->target->hidden)
                return;
            self->target->hidden = true;
            fn_7100209fec(self->target->node, false);
        } else {
            if (!self->target->hidden)
                return;
            self->target->hidden = false;
            fn_7100209fec(self->target->node, true);
        }
    }
}
