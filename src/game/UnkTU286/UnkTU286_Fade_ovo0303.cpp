// Unit 286 (tools/tu_map.py): starts a colour fade on a node: clamps the duration, copies the two colours (alpha
// near 1 snapped to 1), and either applies the target colour at once (and clears the source's value) when the
// duration is about zero, or applies the start colour. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/UnkSphere.h"
#include "game/UnkTU223.h"
#include "game/UnkTU254.h"
#include "game/UnkTU286.h"
#include "types.h"

using base::color::CColor4F;

struct UnkFadeTarget_b256 {
    u8 pad_0x0[0x78];
    void* model;  // 0x78
    u8 pad_0x80[0x103];
    u8 visible;  // 0x183
};
struct UnkFadeTarget2_b256 {
    u8 pad_0x0[0x104];
    f32 alpha;  // 0x104
};

struct UnkFadeSource_b256 {
    u8 pad_0x0[0x78];
    UnkFadeTarget_b256* target;  // 0x78
};

struct UnkFade_b256 {
    u8 pad_0x0[0x24];
    f32 pos_x;  // 0x24
    u8 pad_0x28[0xc];
    f32 pos_y;  // 0x34
    u8 pad_0x38[0xc];
    f32 pos_z;  // 0x44
    u8 pad_0x48[0x38];
    UnkSphere sphere;  // 0x80
    u8 pad_0xd0[0x29];
    bool active;       // 0xf9
    bool pending;      // 0xfa
    bool dirty;        // 0xfb
    u8 pad_0xfc;
    bool fading;       // 0xfd
    u8 pad_0xfe[0xa];
    UnkFadeSource_b256* source;  // 0x108
    f32 elapsed;                 // 0x110
    f32 duration;                // 0x114
    CColor4F from;               // 0x118
    CColor4F to;                 // 0x128
    bool field_0x138;
};
static_assert(__builtin_offsetof(UnkFade_b256, from) == 0x118, "layout");
static_assert(__builtin_offsetof(UnkFade_b256, sphere) == 0x80, "layout");

struct UnkFadeColor;

// Declared elsewhere with (void*, f32, const UnkFadeColor*, const UnkFadeColor*); the colours are CColor4F.
void fn_7100209968(void* node, f32 duration, const UnkFadeColor* from_, const UnkFadeColor* to_) {
    UnkFade_b256* self = static_cast<UnkFade_b256*>(node);
    const CColor4F* from = reinterpret_cast<const CColor4F*>(from_);
    const CColor4F* to = reinterpret_cast<const CColor4F*>(to_);
    f32 d = !(duration < 0.0f) ? duration : 0.0f;
    self->elapsed = 0.0f;
    self->duration = d;
    fn_7100030728(&self->from, from);
    fn_7100030728(&self->to, to);
    self->fading = d > 0.0f;
    if (self->from.a > 0.996f)
        self->from.a = 1.0f;
    if (self->to.a > 0.996f)
        self->to.a = 1.0f;
    f32 hi = d + 0.001f;
    UnkFadeSource_b256* source = self->source;
    if (hi >= 0.0f && d + -0.001f <= 0.0f) {
        f32 r = self->to.r, g = self->to.g, b = self->to.b, a = self->to.a;
        fn_710019c564(source->target, r, g, b, a);
        fn_71001c34f4(self->source, 0.0f);
    } else {
        f32 r = self->from.r, g = self->from.g, b = self->from.b, a = self->from.a;
        fn_710019c564(source->target, r, g, b, a);
    }
}
