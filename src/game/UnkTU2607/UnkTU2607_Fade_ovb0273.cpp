// From unit 2607 of tools/tu_map.py, beside src/game/UnkEnvironmentFade.cpp: the per-frame updates of the
// environment's fading render settings that fade one channel (the update fn_7100f99224 hands them the frame
// time and the object). Each advances its channel's progress, blends the channel's start and end values by it
// (the start at progress 0 or less, the end at 1 or more) and passes the result on: a vector to the render
// manager's pass parameter (channels 3, 18) or to its colour setters (channels 4, 6), back into the object
// (channel 13, which flags its parameter block), or, for channel 9, into the environment manager's light
// object. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU290.h"
#include "game/UnkTU315.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector2D;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkEnvironmentFade;

// A colour channel: the value now, where it starts and where it goes.
struct UnkEnvFadeColorChannel {
    UnkVector4D now;
    UnkVector4D from;
    UnkVector4D to;
};

constexpr u32 kFadeChannels = 29;

struct UnkFadeLayout {
    u8 pad_0x0[0x54];
    UnkEnvFadeColorChannel c3;  // 0x54
    UnkVector4D from4;          // 0x84
    UnkVector4D to4;            // 0x94
    u8 pad_0xa4[0x40];
    UnkVector4D from6;  // 0xe4
    UnkVector4D to6;    // 0xf4
    u8 pad_0x104[0x68];
    CVector2D from9;  // 0x16c
    CVector2D to9;    // 0x174
    u8 pad_0x17c[0x54];
    u8 pad_0x1d0[0x4];
    UnkEnvFadeColorChannel c13;  // 0x1d4
    u8 pad_0x204[0x3c];
    UnkEnvFadeColorChannel c18;  // 0x240
    bool flag_0x270;
    u8 pad_0x271[0xd7];
    f32 progress[kFadeChannels];  // 0x348
    f32 duration[kFadeChannels];  // 0x3bc
    bool done[kFadeChannels];     // 0x430

    // Advances channel `i` by `dt` and gives its progress: 1 once it is done, which is when its duration is
    // (nearly) zero or its progress reaches 0.99.
    f32 Step(u32 i, f32 dt) {
        f32 t;
        bool finished;
        if (duration[i] <= 0.001f || (t = dt / duration[i] + progress[i], progress[i] = t, t >= 0.99f)) {
            t = 1.0f;
            f32* p = &progress[i];
            u32 one = 0x3f800000;
            // Keeps the progress address and the constant in registers, as the original stores them.
            asm("" : "+r"(p), "+r"(one));
            *reinterpret_cast<u32*>(p) = one;
            finished = true;
        } else {
            finished = false;
        }
        done[i] = finished;
        return t;
    }
};
static_assert(__builtin_offsetof(UnkFadeLayout, from6) == 0xe4, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, from9) == 0x16c, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, c13) == 0x1d4, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, c18) == 0x240, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");

static inline UnkFadeLayout* L(UnkEnvironmentFade* self) { return reinterpret_cast<UnkFadeLayout*>(self); }

// A vector of four faded from `from` to `to`.
static inline UnkVector4D FadeLerp4(f32 t, const UnkVector4D& from, const UnkVector4D& to) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    UnkVector4D a = fn_710007b5c0(&from, 1.0f - t);
    UnkVector4D b = fn_710007b5c0(&to, t);
    return fn_710007b5ec(&a, &b);
}

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

// Channel 3: the colour sent to the pass parameter 0x86fdd4eeb6de9890.
void fn_7100f9a2cc(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = L(fade);
    f32 t = self->Step(3, dt);
    UnkVector4D r = FadeLerp4(t, self->c3.now, self->c3.to);
    UnkVector4D v(r);
    fn_7100219294(g_unk_7101d47300, 0x86fdd4eeb6de9890, &v.x, 0);
}

// The render manager's colour setter that fn_7100f9a408 feeds (not decompiled).
void fn_7100219e6c(UnkRenderManager* self, const CColor4F* color);

// Channel 4: a colour, each component at least zero, to the render manager.
void fn_7100f9a408(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = L(fade);
    f32 t = self->Step(4, dt);
    UnkVector4D r = FadeLerp4(t, self->from4, self->to4);
    CColor4F color;
    color.r = NonNegative(r.x);
    color.g = NonNegative(r.y);
    color.b = NonNegative(r.z);
    color.a = NonNegative(r.w);
    fn_7100219e6c(g_unk_7101d47300, &color);
}

struct UnkFadeColor {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

// Channel 6: a colour, each component at least zero, as the vector the render manager's layer 1 takes.
void fn_7100f9a6d8(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = L(fade);
    f32 t = self->Step(6, dt);
    UnkVector4D r = FadeLerp4(t, self->from6, self->to6);
    CColor4F color;
    color.r = NonNegative(r.x);
    color.g = NonNegative(r.y);
    color.b = NonNegative(r.z);
    color.a = NonNegative(r.w);
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkVector4D v = fn_7100030878(&color);
    fn_7100219fe4(manager, 1, reinterpret_cast<const UnkFadeColor&>(v));
}

// Channel 13: fades the colour in place (from itself to its end) and flags the first parameter block.
void fn_7100f9ada4(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = L(fade);
    f32 t = self->Step(13, dt);
    UnkVector4D r = FadeLerp4(t, self->c13.now, self->c13.to);
    fn_710007b518(&self->c13.now, &r);
    self->flag_0x270 = true;
}

// Channel 18: the colour sent to the pass parameter 0xdaafacbdfd7d600f.
void fn_7100f9aeec(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = L(fade);
    f32 t = self->Step(18, dt);
    UnkVector4D r = FadeLerp4(t, self->c18.now, self->c18.to);
    UnkVector4D v(r);
    fn_7100219294(g_unk_7101d47300, 0xdaafacbdfd7d600f, &v.x, 0);
}

// The environment manager (0x4c8 bytes, created on first use) and the light object at its +0x498.
struct UnkEnvFadeTarget {
    u8 pad_0x0[0x7c];
    f32 field_0x7c;
};

struct UnkEnvFadeOwnerInner {
    u8 pad_0x0[0x78];
    UnkEnvFadeTarget* target;  // 0x78
};

struct UnkEnvFadeOwner {
    u8 pad_0x0[0x28];
    u8 member[0x118];             // 0x28
    UnkEnvFadeOwnerInner* inner;  // 0x140
};

struct UnkEnvManager {
    u8 pad_0x0[0x498];
    UnkEnvFadeOwner* owner;  // 0x498
};

static inline u32 Bits(f32 v) {
    u32 b;
    __builtin_memcpy(&b, &v, 4);
    return b;
}

// Channel 9: the blended pair goes to the light object, the first into its target's +0x7c and the second
// to its member at +0x28.
void fn_7100f9ac60(f32 dt, UnkEnvironmentFade* fade) {
    UnkFadeLayout* self = L(fade);
    f32 t = self->Step(9, dt);
    UnkEnvManager* manager = static_cast<UnkEnvManager*>(g_unk_7101d47368);
    if (!manager) {
        manager = static_cast<UnkEnvManager*>(fn_71000839a0(0x4c8, 0x3f));
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
        t = self->progress[9];
    }
    UnkEnvFadeOwner* owner = manager->owner;
    u32 x;
    f32 y;
    if (t >= 1.0f) {
        x = Bits(self->to9.x);
        y = self->to9.y;
    } else if (t < 0.0f) {
        x = Bits(self->from9.x);
        y = self->from9.y;
    } else {
        f32 k = 1.0f - t;
        f32 a = k * self->from9.x;
        f32 b = k * self->from9.y;
        f32 c = t * self->to9.x;
        f32 d = t * self->to9.y;
        f32 sum_x = a + c;
        y = b + d;
        x = Bits(sum_x);
    }
    if (owner) {
        *reinterpret_cast<u32*>(&owner->inner->target->field_0x7c) = x;
        fn_710020c9ac(owner->member, y);
    }
}
