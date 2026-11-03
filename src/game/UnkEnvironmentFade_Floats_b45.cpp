// From unit 2607 of tools/tu_map.py, beside src/game/UnkEnvironmentFade.cpp: the setters that start
// one channel's fade of the environment's fading render settings. Each starts the channel (its
// duration, its progress: 1 at once when the duration is nearly zero, and its done flag cleared),
// takes the value it fades from from the value now and stores where it goes; some first set a flag.
// The layout below covers only the channels set here. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

// As src/game/UnkEnvironmentVisuals.cpp declares the setters.
struct UnkEnvironmentFade;

namespace {

constexpr u32 kFadeChannels = 29;

// A float channel: the value now, where it starts and where it goes.
struct UnkFadeFloat {
    f32 now;
    f32 from;
    f32 to;
};
// The same with the value now last.
struct UnkFadeFloatB {
    f32 from;
    f32 to;
    f32 now;
};

// A colour channel.
struct UnkFadeColor {
    CVector4D now;
    CVector4D from;
    CVector4D to;
};

struct UnkFadeLayout {
    u8 pad_0x0[0x3c];
    CVector2D now2;   // 0x3c
    CVector2D from2;  // 0x44
    CVector2D to2;    // 0x4c
    UnkFadeColor c3;  // 0x54
    u8 pad_0x84[0xf8];
    CVector3D from10;  // 0x17c
    CVector3D to10;    // 0x188
    CVector3D from11;  // 0x194
    CVector3D to11;    // 0x1a0
    CVector3D from12;  // 0x1ac
    CVector3D to12;    // 0x1b8
    CVector3D now12;   // 0x1c4
    u8 pad_0x1d0[0x4];
    UnkFadeColor c13;    // 0x1d4
    UnkFadeFloat f14;    // 0x204
    UnkFadeFloat f16;    // 0x210
    UnkFadeFloat f17;    // 0x21c
    CVector2D now15;     // 0x228
    CVector2D from15;    // 0x230
    CVector2D to15;      // 0x238
    UnkFadeColor c18;    // 0x240
    bool flag_0x270;
    u8 pad_0x271[0x17];
    UnkFadeFloatB f19;   // 0x288
    UnkFadeFloatB f20;   // 0x294
    bool flag_0x2a0;
    u8 pad_0x2a1[0x3];
    UnkFadeFloat f21;    // 0x2a4
    UnkFadeFloat f22;    // 0x2b0
    UnkFadeFloat f23;    // 0x2bc
    UnkFadeFloat f24;    // 0x2c8
    UnkFadeFloat f25;    // 0x2d4
    UnkFadeColor c26;    // 0x2e0
    UnkFadeFloat f27;    // 0x310
    bool flag_0x31c;
    u8 pad_0x31d[0x3];
    UnkFadeFloat f28[3];  // 0x320
    bool flag_0x344;
    u8 pad_0x345[0x3];
    f32 progress[kFadeChannels];  // 0x348
    f32 duration[kFadeChannels];  // 0x3bc
    bool done[kFadeChannels];     // 0x430

    void Start(u32 i, f32 time) {
        StartTime(i, time);
        StartProgress(i, time);
    }
    // Start in two halves, for the setters whose fields' addresses are taken between them.
    void StartTime(u32 i, f32 time) {
        duration[i] = time;
        done[i] = false;
    }
    void StartProgress(u32 i, f32 time) {
        bool lo_ok = time - 0.001f <= 0.0f;
        bool hi_ok = time + 0.001f >= 0.0f;
        progress[i] = hi_ok && lo_ok ? 1.0f : 0.0f;
    }
};
static_assert(__builtin_offsetof(UnkFadeLayout, c18) == 0x240, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, f27) == 0x310, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, progress) == 0x348, "UnkFadeLayout placeholder layout");
static_assert(__builtin_offsetof(UnkFadeLayout, done) == 0x430, "UnkFadeLayout placeholder layout");

__attribute__((always_inline)) inline UnkFadeLayout* L(UnkEnvironmentFade* self) {
    return reinterpret_cast<UnkFadeLayout*>(self);
}

template <class F>
__attribute__((always_inline)) inline void Fade(UnkFadeLayout* self, u32 channel, F* f, f32 to, f32 time) {
    self->Start(channel, time);
    f->from = f->now;
    f->to = to;
}

template <class F>
__attribute__((always_inline)) inline void FadeFlagged(UnkFadeLayout* self, bool* flag, u32 channel, F* f, f32 to,
                                                       f32 time) {
    *flag = true;
    Fade(self, channel, f, to, time);
}

__attribute__((always_inline)) inline void FadeColor(UnkFadeLayout* self, u32 channel, UnkFadeColor* c,
                                                     const CVector4D* to, f32 time) {
    self->StartTime(channel, time);
    const CVector4D* now = &c->now;
    self->StartProgress(channel, time);
    fn_710007b518(&c->from, now);
    fn_710007b518(&c->to, to);
}

}  // namespace

void fn_7100f987c4(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x270, 14, &s->f14, to, time);
}
void fn_7100f988f8(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    Fade(s, 16, &s->f16, to, time);
}
void fn_7100f98948(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    Fade(s, 17, &s->f17, to, time);
}
void fn_7100f98e64(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x2a0, 19, &s->f19, to, time);
}
void fn_7100f98ebc(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x2a0, 20, &s->f20, to, time);
}
void fn_7100f98f28(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x31c, 21, &s->f21, to, time);
}
void fn_7100f98f80(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x31c, 22, &s->f22, to, time);
}
void fn_7100f98fd8(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x31c, 23, &s->f23, to, time);
}
void fn_7100f99030(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x31c, 24, &s->f24, to, time);
}
void fn_7100f99088(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x31c, 25, &s->f25, to, time);
}
void fn_7100f9915c(f32 to, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    FadeFlagged(s, &s->flag_0x31c, 27, &s->f27, to, time);
}

void fn_7100f98128(UnkEnvironmentFade* self, UnkVector4D to, f32 time) {
    UnkFadeLayout* s = L(self);
    FadeColor(s, 3, &s->c3, &to, time);
}
void fn_7100f9873c(f32 time, UnkEnvironmentFade* self, const UnkVector4D* to) {
    UnkFadeLayout* s = L(self);
    FadeColor(s, 13, &s->c13, to, time);
}
void fn_7100f98884(f32 time, UnkEnvironmentFade* self, const UnkVector4D* to) {
    UnkFadeLayout* s = L(self);
    FadeColor(s, 18, &s->c18, to, time);
}

void fn_7100f991b4(f32 a, f32 b, f32 c, f32 time, UnkEnvironmentFade* self) {
    UnkFadeLayout* s = L(self);
    s->flag_0x344 = true;
    s->Start(28, time);
    s->f28[0].from = s->f28[0].now;
    s->f28[1].from = s->f28[1].now;
    s->f28[2].from = s->f28[2].now;
    s->f28[0].to = a;
    s->f28[1].to = b;
    s->f28[2].to = c;
}
