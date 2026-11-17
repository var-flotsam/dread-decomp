// From unit 2607 of tools/tu_map.py, beside src/game/UnkEnvironmentFade.cpp: the per-frame update of channel 8
// of the environment's fading render settings, three colours. It advances the channel, blends each colour from
// its start to its end (each component at least zero) and, when the environment manager (created on first use)
// has a light object, hands the three colours to it. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/reflection.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkEnvironmentFade;

constexpr u32 kLightFadeChannels_lf9a844 = 29;

// A colour faded from `from` to `to`.
struct UnkLightFadePair_lf9a844 {
    UnkVector4D from;
    UnkVector4D to;
};

struct UnkLightFadeLayout_lf9a844 {
    u8 pad_0x0[0x10c];
    UnkLightFadePair_lf9a844 colors[3];  // 0x10c
    u8 pad_0x16c[0x348 - 0x16c];
    f32 progress[kLightFadeChannels_lf9a844];  // 0x348
    f32 duration[kLightFadeChannels_lf9a844];  // 0x3bc
    bool done[kLightFadeChannels_lf9a844];     // 0x430

    // Advances channel `i` by `dt`: it is done when its duration is (nearly) zero or its progress reaches
    // 0.99, and its progress is then 1.
    void Advance(u32 i, f32 dt) {
        if (duration[i] <= 0.001f) {
            progress[i] = 1.0f;
            done[i] = true;
        } else {
            progress[i] = dt / duration[i] + progress[i];
            if (progress[i] >= 0.99f) {
                progress[i] = 1.0f;
                done[i] = true;
            } else {
                done[i] = false;
            }
        }
    }
};
static_assert(__builtin_offsetof(UnkLightFadeLayout_lf9a844, progress) == 0x348, "layout");
static_assert(__builtin_offsetof(UnkLightFadeLayout_lf9a844, done) == 0x430, "layout");

// The light's colours.
struct UnkLightColors_lf9a844 {
    u8 pad_0x0[0x3c];
    CVector4D field_0x3c;
    u8 pad_0x4c[0x10];
    CVector4D field_0x5c;
    CVector4D field_0x6c;
};

struct UnkLightInner_lf9a844 {
    u8 pad_0x0[0x78];
    UnkLightColors_lf9a844* colors;  // 0x78
};

struct UnkLight_lf9a844 {
    u8 pad_0x0[0x140];
    UnkLightInner_lf9a844* inner;  // 0x140
};

// The environment manager (0x4c8 bytes, created on first use) and the light object at its +0x498.
struct UnkLightEnvManager_lf9a844 {
    u8 pad_0x0[0x498];
    UnkLight_lf9a844* light;  // 0x498
};

static inline UnkVector4D FadeLerp4(f32 t, const UnkVector4D& from, const UnkVector4D& to) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    UnkVector4D a = from * (1.0f - t);
    UnkVector4D b = to * t;
    return a + b;
}

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

static inline void ToColor(CColor4F* color, const UnkVector4D& v) {
    color->r = NonNegative(v.x);
    color->g = NonNegative(v.y);
    color->b = NonNegative(v.z);
    color->a = NonNegative(v.w);
}

// Sets a light colour: the colour as a vector, each component at least zero.
static inline void SetColor(CVector4D* dst, const CColor4F& color) {
    UnkVector4D v = fn_7100030878(&color);
    UnkVector4D clamped(NonNegative(v.x), NonNegative(v.y), NonNegative(v.z), NonNegative(v.w));
    fn_710007b518(dst, &clamped);
}

void fn_7100f9a844(f32 dt, UnkEnvironmentFade* fade) {
    UnkLightFadeLayout_lf9a844* self = reinterpret_cast<UnkLightFadeLayout_lf9a844*>(fade);
    self->Advance(8, dt);
    auto* manager = static_cast<UnkLightEnvManager_lf9a844*>(g_unk_7101d47368);
    if (!manager) {
        manager = static_cast<UnkLightEnvManager_lf9a844*>(fn_71000839a0(0x4c8, 0x3f));
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    UnkLight_lf9a844* light = manager->light;
    CColor4F first;
    ToColor(&first, FadeLerp4(self->progress[8], self->colors[0].from, self->colors[0].to));
    CColor4F second;
    ToColor(&second, FadeLerp4(self->progress[8], self->colors[1].from, self->colors[1].to));
    CColor4F third;
    ToColor(&third, FadeLerp4(self->progress[8], self->colors[2].from, self->colors[2].to));
    if (light) {
        SetColor(&light->inner->colors->field_0x3c, first);
        SetColor(&light->inner->colors->field_0x5c, second);
        SetColor(&light->inner->colors->field_0x6c, third);
    }
}
