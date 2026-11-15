// UnkTU2656: starts a fade on the node at +0x18 of the object's owner, clamping both colours to be
// non-negative and scaling their alpha. Names and layouts are placeholders.
#include "game/UnkTU286.h"
#include "types.h"

struct UnkFadeColor {
    f32 r, g, b, a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct UnkFadeOwner_l053968 {
    u8 pad_0x0[0x18];
    u8 node;
};

struct UnkFadeUser_l053968 {
    UnkFadeOwner_l053968* owner;
};

struct UnkRgb_l053968 {
    f32 r, g, b;
};

static inline f32 UnkClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

static inline UnkFadeColor UnkMakeFade(const UnkRgb_l053968* c, f32 alpha) {
    return UnkFadeColor(UnkClampPositive(c->r), UnkClampPositive(c->g), UnkClampPositive(c->b),
                        UnkClampPositive(alpha * 1.2f));
}

void fn_7101053968(UnkFadeUser_l053968* self, f32 duration, const UnkRgb_l053968* to, f32 toAlpha,
                   const UnkRgb_l053968* from, f32 fromAlpha) {
    UnkFadeColor a = UnkMakeFade(to, toAlpha);
    UnkFadeColor b = UnkMakeFade(from, fromAlpha);
    fn_7100209968(&self->owner->node, duration, &a, &b);
}
