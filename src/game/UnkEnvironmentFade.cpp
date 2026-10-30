// From unit 2607 of tools/tu_map.py (0x7100f97d90-0x7100f9b32c, no reflected class; its static
// initialiser builds the ids "binaries" and "sources"): a set of render settings that fade from
// the values the render manager has now to new ones over a time, one channel at a time. Each
// channel has its duration, its progress (0 while it runs, 1 when it is done at once) and a flag,
// in three arrays of 29; the values faded live before them. fn_7100d8e398 applies a preset with
// these setters (the object at +0x28 of its first argument). Names and layouts are placeholders.
// Holds fn_7100f97f64, fn_7100f9800c, fn_7100f9819c, fn_7100f98244, fn_7100f98370 and
// fn_7100f98410 (start a channel's fade) and fn_7100f989f0 (sets the texture by its path).
#include "base/UnkResKey.h"
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The colours faded are vectors: their assignment is the vector's (fn_710007b518, as
// src/game/CMinimapManager.cpp's colour), and a local one is zeroed and copied member-wise as the
// original's CVector4D is (so one returned by value comes back in memory).
using UnkEnvFadeColor = base::math::UnkVector4D;

// A float faded from `from` to `to`.
struct UnkFadePair {
    f32 from;
    f32 to;
};

constexpr u32 kFadeChannels = 29;

struct UnkEnvironmentFade {
    UnkEnvFadeColor field_0x0;   // channel 0: where it starts
    UnkEnvFadeColor field_0x10;  // channel 0: the value now
    UnkEnvFadeColor field_0x20;  // channel 0: where it goes
    f32 field_0x30;              // channel 1: where it starts
    f32 field_0x34;              // channel 1: the value now
    f32 field_0x38;              // channel 1: where it goes
    u8 pad_0x3c[0x48];
    UnkEnvFadeColor field_0x84;  // channel 4: where it starts
    UnkEnvFadeColor field_0x94;  // channel 4: where it goes
    UnkFadePair field_0xa4[8];  // channel 5: two sets of four floats, interleaved
    UnkEnvFadeColor field_0xe4;  // channel 6: where it starts
    UnkEnvFadeColor field_0xf4;  // channel 6: where it goes
    UnkFadePair field_0x104;  // channel 7
    u8 pad_0x10c[0x165];
    u8 field_0x271;  // passed with the texture
    u8 pad_0x272[0x6];
    CFilePathStrId field_0x278;  // the texture's path
    UnkResRef field_0x280;       // the texture
    u8 pad_0x288[0xc0];
    f32 progress[kFadeChannels];
    f32 duration[kFadeChannels];
    bool done[kFadeChannels];

    // Starts channel `i`: it is done at once when the duration is (nearly) zero.
    void Start(u32 i, f32 time) {
        duration[i] = time;
        done[i] = false;
        bool lo_ok = time - 0.001f <= 0.0f;
        bool hi_ok = time + 0.001f >= 0.0f;
        progress[i] = hi_ok && lo_ok ? 1.0f : 0.0f;
    }
};
static_assert(__builtin_offsetof(UnkEnvironmentFade, field_0x104) == 0x104, "UnkEnvironmentFade placeholder layout");
static_assert(__builtin_offsetof(UnkEnvironmentFade, progress) == 0x348, "UnkEnvironmentFade placeholder layout");
static_assert(__builtin_offsetof(UnkEnvironmentFade, duration) == 0x3bc, "UnkEnvironmentFade placeholder layout");
static_assert(__builtin_offsetof(UnkEnvironmentFade, done) == 0x430, "UnkEnvironmentFade placeholder layout");

// The render manager's current values.
void fn_7100219fa0(UnkRenderManager* self, UnkEnvFadeColor* out, u32 arg);

// NON_MATCHING(minor): the original schedules the render manager's GOT page (adrp) before the
// zero argument and the progress store instead of after them (independent instructions).
void fn_7100f97f64(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to) {
    self->Start(0, time);
    UnkEnvFadeColor now;
    fn_7100219fa0(g_unk_7101d47300, &now, 0);
    fn_710007b518(&self->field_0x0, &now);
    fn_710007b518(&self->field_0x10, &self->field_0x0);
    fn_710007b518(&self->field_0x20, to);
}

// The render manager's current value for channel 1 (generated, src/lifted/: it forwards to the
// pass list's fn_71001fd94c).
f32 fn_7100219fd4(UnkRenderManager* self, u32 arg);

void fn_7100f9800c(f32 to, f32 time, UnkEnvironmentFade* self) {
    self->Start(1, time);
    f32 now = fn_7100219fd4(g_unk_7101d47300, 0);
    self->field_0x30 = now;
    self->field_0x34 = now;
    self->field_0x38 = to;
}

void fn_7100219e74(UnkRenderManager* self, CColor4F* inout);

void fn_7100f9819c(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to) {
    self->Start(4, time);
    CColor4F now(1.0f, 1.0f, 1.0f, 1.0f);
    fn_7100219e74(g_unk_7101d47300, &now);
    UnkEnvFadeColor from = fn_7100030878(&now);
    fn_710007b518(&self->field_0x84, &from);
    fn_710007b518(&self->field_0x94, to);
}

const UnkEnvFadeColor* fn_71002192b8(UnkRenderManager* self);
const UnkEnvFadeColor* fn_71002192c4(UnkRenderManager* self);

void fn_7100f98244(base::math::CVector4D a, base::math::CVector4D b, UnkEnvironmentFade* self, f32 time) {
    self->Start(5, time);
    UnkEnvFadeColor now = *fn_71002192b8(g_unk_7101d47300);
    self->field_0xa4[0].from = now.x;
    self->field_0xa4[1].from = now.y;
    self->field_0xa4[2].from = now.z;
    self->field_0xa4[3].from = now.w;
    fn_710007b518(&now, fn_71002192c4(g_unk_7101d47300));
    self->field_0xa4[4].from = now.x;
    self->field_0xa4[5].from = now.y;
    self->field_0xa4[6].from = now.z;
    self->field_0xa4[7].from = now.w;
    self->field_0xa4[0].to = a.x;
    self->field_0xa4[1].to = a.y;
    self->field_0xa4[2].to = a.z;
    self->field_0xa4[3].to = a.w;
    self->field_0xa4[4].to = b.x;
    self->field_0xa4[5].to = b.y;
    self->field_0xa4[6].to = b.z;
    self->field_0xa4[7].to = b.w;
}

void fn_7100219fec(UnkRenderManager* self, u32 arg, f32* out_value, UnkEnvFadeColor* out_color);

// NON_MATCHING(minor): the original schedules the render manager's GOT page (adrp) before the
// argument addresses and the progress store instead of after the local's zeroing (independent
// instructions).
void fn_7100f98370(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to) {
    self->Start(6, time);
    f32 value;
    UnkEnvFadeColor now;
    fn_7100219fec(g_unk_7101d47300, 1, &value, &now);
    fn_710007b518(&self->field_0xe4, &now);
    fn_710007b518(&self->field_0xf4, to);
}

// NON_MATCHING(minor): the original computes the scalar out-argument's address (sub x2) right
// after the flag store and saves `self` before zeroing the local colour (independent
// instructions scheduled in another order).
void fn_7100f98410(f32 to, f32 time, UnkEnvironmentFade* self) {
    self->Start(7, time);
    f32 now;
    UnkEnvFadeColor color;
    fn_7100219fec(g_unk_7101d47300, 1, &now, &color);
    self->field_0x104.from = now;
    self->field_0x104.to = to;
}

// UnkResRef's assignment with the other pointer read first.
static inline void Assign(UnkResRef* self, const UnkResRef& other) {
    UnkCountedResource* p = other.ptr;
    if (self->ptr != p) {
        if (p)
            ++p->ref_count;
        UnkCountedResource* old = self->ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        self->ptr = p;
    }
}

// The texture parameter as the render manager takes it: the flag at +0x271 and the texture.
struct UnkTextureParam {
    u64 flag;
    UnkCountedResource* texture;
};

// Sets the texture's path: loads the texture it names (none for the empty path) and passes it to
// the render manager with the flag at +0x271.
void fn_7100f989f0(UnkEnvironmentFade* self, const CFilePathStrId& path) {
    CFilePathStrId* own = &self->field_0x278;
    if (own->data == path.data)
        return;
    {
        UnkStringData* d = path.data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (own->data != &pool->empty) {
            --own->data->ref_count;
            if (own->data->ref_count == 0)
                fn_71000803a4(pool, own->data);
        }
        own->data = d;
    }
    if (own->data == &g_unk_7101d47200->empty) {
        if (UnkCountedResource* old = self->field_0x280.ptr) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
            self->field_0x280.ptr = nullptr;
        }
    } else {
        UnkResRegistry* registry = g_unk_7101d472f8;
        UnkResKey key(g_unk_716e4540f8.name, *own);
        UnkResKey found = fn_71000ea448(registry, &key, nullptr);
        if (found.type.data != &g_unk_7101d47108->empty && found.path.data != &g_unk_7101d47200->empty)
            Assign(&self->field_0x280, fn_71000ead80(registry, &found, 0x14, nullptr));
    }
    UnkTextureParam param;
    param.flag = self->field_0x271;
    param.texture = self->field_0x280.ptr;
    fn_7100219294(g_unk_7101d47300, 0xbd88e4a69dc0e5c9, reinterpret_cast<const f32*>(&param), 0);
}

// The faded vectors: copied member by member (the original copies the components as words).
struct UnkFadeVec2 {
    f32 x;
    f32 y;

    UnkFadeVec2(f32 x, f32 y) : x(x), y(y) {}
    UnkFadeVec2(const UnkFadeVec2& other) : x(other.x), y(other.y) {}
    UnkFadeVec2& operator=(const UnkFadeVec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

struct UnkFadeVec3 {
    f32 x;
    f32 y;
    f32 z;

    UnkFadeVec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkFadeVec3(const UnkFadeVec3& other) : x(other.x), y(other.y), z(other.z) {}
    UnkFadeVec3& operator=(const UnkFadeVec3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

static inline UnkFadeVec2 operator*(f32 s, const UnkFadeVec2& v) {
    return UnkFadeVec2(s * v.x, s * v.y);
}
static inline UnkFadeVec2 operator+(const UnkFadeVec2& a, const UnkFadeVec2& b) {
    return UnkFadeVec2(a.x + b.x, a.y + b.y);
}
static inline UnkFadeVec3 operator*(f32 s, const UnkFadeVec3& v) {
    return UnkFadeVec3(s * v.x, s * v.y, s * v.z);
}
static inline UnkFadeVec3 operator+(const UnkFadeVec3& a, const UnkFadeVec3& b) {
    return UnkFadeVec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// A float faded with its current value last.
struct UnkFadeValueLast {
    f32 from;
    f32 to;
    f32 value;
};

// The whole object as the per-frame update (fn_7100f99224) sees it: the values each channel
// fades, the parameter blocks they feed and their dirty flags, at their offsets in
// UnkEnvironmentFade (whose layout above names fewer members).
struct UnkEnvFadeState {
    u8 pad_0x0[0x34];
    UnkFadePair field_0x34;  // channel 1
    UnkFadeVec2 field_0x3c;  // channel 2: where it starts
    u8 pad_0x44[0x8];
    UnkFadeVec2 field_0x4c;  // channel 2: where it goes (also the first block's range fallback)
    u8 pad_0x54[0xb0];
    UnkFadePair field_0x104;  // channel 7
    u8 pad_0x10c[0x70];
    UnkFadeVec3 field_0x17c;  // channel 10: where it starts
    UnkFadeVec3 field_0x188;  // channel 10: where it goes
    UnkFadeVec3 field_0x194;  // channel 11: where it starts
    UnkFadeVec3 field_0x1a0;  // channel 11: where it goes
    UnkFadeVec3 field_0x1ac;  // channel 12: where it starts
    UnkFadeVec3 field_0x1b8;  // channel 12: where it goes
    UnkFadeVec3 field_0x1c4;  // channel 12: the value now
    bool field_0x1d0;         // the first block's first parameter (1 or 0)
    f32 field_0x1d4[4];       // the first block's next four parameters
    u8 pad_0x1e4[0x20];
    f32 field_0x204;          // channel 14: the value now
    UnkFadePair field_0x208;  // channel 14
    f32 field_0x210;          // channel 16: the value now (the range's near end)
    UnkFadePair field_0x214;  // channel 16
    f32 field_0x21c;          // channel 17: the value now (the range's far end)
    UnkFadePair field_0x220;  // channel 17
    UnkFadeVec2 field_0x228;  // channel 15: the value now, faded from itself
    u8 pad_0x230[0x8];
    UnkFadeVec2 field_0x238;  // channel 15: where it goes
    u8 pad_0x240[0x30];
    bool field_0x270;  // the first block changed
    u8 pad_0x271[0x17];
    UnkFadeValueLast field_0x288;  // channel 19
    UnkFadeValueLast field_0x294;  // channel 20
    bool field_0x2a0;              // the second block changed
    bool field_0x2a1;              // the third block's first parameter (1 or 0)
    u8 pad_0x2a2[0x2];
    f32 field_0x2a4;          // channel 21: the value now
    UnkFadePair field_0x2a8;  // channel 21
    f32 field_0x2b0;          // channel 22: the value now
    UnkFadePair field_0x2b4;  // channel 22
    f32 field_0x2bc;          // channel 23: the value now
    UnkFadePair field_0x2c0;  // channel 23
    f32 field_0x2c8;          // channel 24: the value now
    UnkFadePair field_0x2cc;  // channel 24
    f32 field_0x2d4;          // channel 25: the value now
    UnkFadePair field_0x2d8;  // channel 25
    f32 field_0x2e0[4];       // the third block's parameters 6 to 9
    u8 pad_0x2f0[0x20];
    f32 field_0x310;          // channel 27: the value now
    UnkFadePair field_0x314;  // channel 27
    bool field_0x31c;         // the third block changed
    u8 pad_0x31d[0x27];
    bool field_0x344;  // fn_7100f9b170 runs
    u8 pad_0x345[0x3];
    f32 progress[kFadeChannels];
    f32 duration[kFadeChannels];
    bool done[kFadeChannels];

    // Advances channel `i` by `dt`: it is done when its duration is (nearly) zero or its progress
    // reaches 0.99, and its progress is then 1.
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
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x104) == 0x104, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x1d4) == 0x1d4, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x204) == 0x204, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x270) == 0x270, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x288) == 0x288, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x2e0) == 0x2e0, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, field_0x31c) == 0x31c, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, progress) == 0x348, "UnkEnvFadeState layout");
static_assert(__builtin_offsetof(UnkEnvFadeState, done) == 0x430, "UnkEnvFadeState layout");

// The value at `t` between `from` and `to`, clamped to them.
static inline f32 FadeLerp(f32 t, f32 from, f32 to) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

static inline UnkFadeVec2 FadeLerp(f32 t, const UnkFadeVec2& from, const UnkFadeVec2& to) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

// Vectors of three are clamped at the end only.
static inline UnkFadeVec3 FadeLerp(f32 t, const UnkFadeVec3& from, const UnkFadeVec3& to) {
    if (t >= 1.0f)
        return to;
    return (1.0f - t) * from + t * to;
}

// The channels the update hands to functions of their own (each takes the frame time and the
// object; not decompiled yet).
void fn_7100f9a190(f32 dt, UnkEnvironmentFade* self);  // channel 0
void fn_7100f9a2cc(f32 dt, UnkEnvironmentFade* self);  // channel 3
void fn_7100f9a408(f32 dt, UnkEnvironmentFade* self);  // channel 4
void fn_7100f9a560(f32 dt, UnkEnvironmentFade* self);  // channel 5
void fn_7100f9a6d8(f32 dt, UnkEnvironmentFade* self);  // channel 6
void fn_7100f9a844(f32 dt, UnkEnvironmentFade* self);  // channel 8
void fn_7100f9ac60(f32 dt, UnkEnvironmentFade* self);  // channel 9
void fn_7100f9ada4(f32 dt, UnkEnvironmentFade* self);  // channel 13
void fn_7100f9aeec(f32 dt, UnkEnvironmentFade* self);  // channel 18
void fn_7100f9b038(f32 dt, UnkEnvironmentFade* self);  // channel 26
void fn_7100f9b170(f32 dt, UnkEnvironmentFade* self);  // what runs while +0x344 is set

// The render manager's setters for the faded values: one value for a view (fn_7100219fcc,
// fn_7100219fdc; they forward to the pass list), the view's two range values
// (src/game/UnkRenderManager.cpp) and two vectors of its settings block.
void fn_7100219fcc(f32 value, UnkRenderManager* self, u32 view);
void fn_7100219fdc(f32 value, UnkRenderManager* self, u32 view);
void fn_7100219e88(UnkRenderManager* self, u32 view, f32 x, f32 y);
void fn_7100219ff4(UnkRenderManager* self, const UnkFadeVec3* value);
void fn_710021a020(UnkRenderManager* self, const UnkFadeVec3* value);

// The per-frame update: advances every channel that is not done by `dt` and hands the values to
// the render manager, the pass parameters in three blocks that are sent when one of their
// channels changed.
void fn_7100f99224(f32 dt, UnkEnvironmentFade* fade) {
    UnkEnvFadeState* self = reinterpret_cast<UnkEnvFadeState*>(fade);
    if (!self->done[0])
        fn_7100f9a190(dt, fade);
    if (!self->done[1]) {
        self->Advance(1, dt);
        fn_7100219fcc(FadeLerp(self->progress[1], self->field_0x34.from, self->field_0x34.to), g_unk_7101d47300, 0);
    }
    if (!self->done[2]) {
        self->Advance(2, dt);
        // The same fade as FadeLerp's, written out (the original reads the ends in each branch).
        f32 t = self->progress[2];
        UnkFadeVec2 v = t >= 1.0f ? self->field_0x4c
                        : t < 0.0f ? self->field_0x3c
                                   : (1.0f - t) * self->field_0x3c + t * self->field_0x4c;
        fn_7100219e88(g_unk_7101d47300, 0, v.x, v.y);
        self->field_0x270 = true;
    }
    if (!self->done[3])
        fn_7100f9a2cc(dt, fade);
    if (!self->done[4])
        fn_7100f9a408(dt, fade);
    if (!self->done[5])
        fn_7100f9a560(dt, fade);
    if (!self->done[6])
        fn_7100f9a6d8(dt, fade);
    if (!self->done[7]) {
        self->Advance(7, dt);
        fn_7100219fdc(FadeLerp(self->progress[7], self->field_0x104.from, self->field_0x104.to), g_unk_7101d47300, 1);
    }
    if (!self->done[8])
        fn_7100f9a844(dt, fade);
    if (!self->done[9])
        fn_7100f9ac60(dt, fade);
    if (!self->done[10]) {
        self->Advance(10, dt);
        UnkFadeVec3 v = FadeLerp(self->progress[10], self->field_0x17c, self->field_0x188);
        fn_7100219ff4(g_unk_7101d47300, &v);
    }
    if (!self->done[11]) {
        self->Advance(11, dt);
        UnkFadeVec3 v = FadeLerp(self->progress[11], self->field_0x194, self->field_0x1a0);
        fn_710021a020(g_unk_7101d47300, &v);
    }
    if (!self->done[12]) {
        self->Advance(12, dt);
        self->field_0x1c4 = FadeLerp(self->progress[12], self->field_0x1ac, self->field_0x1b8);
        f32 param[4] = {self->field_0x1c4.x, self->field_0x1c4.y, self->field_0x1c4.z, 1.0f};
        fn_7100219294(g_unk_7101d47300, 0x9407baf9eae3a922, param, 0);
    }
    if (!self->done[13])
        fn_7100f9ada4(dt, fade);
    if (!self->done[14]) {
        self->Advance(14, dt);
        self->field_0x204 = FadeLerp(self->progress[14], self->field_0x208.from, self->field_0x208.to);
        self->field_0x270 = true;
    }
    if (!self->done[16]) {
        self->Advance(16, dt);
        self->field_0x210 = FadeLerp(self->progress[16], self->field_0x214.from, self->field_0x214.to);
        self->field_0x270 = true;
    }
    if (!self->done[17]) {
        self->Advance(17, dt);
        self->field_0x21c = FadeLerp(self->progress[17], self->field_0x220.from, self->field_0x220.to);
        self->field_0x270 = true;
    }
    if (!self->done[15]) {
        self->Advance(15, dt);
        self->field_0x228 = FadeLerp(self->progress[15], self->field_0x228, self->field_0x238);
        self->field_0x270 = true;
    }
    if (self->field_0x270) {
        f32 param[10];
        param[0] = self->field_0x1d0 ? 1.0f : 0.0f;
        param[1] = self->field_0x1d4[0];
        param[2] = self->field_0x1d4[1];
        param[3] = self->field_0x1d4[2];
        param[4] = self->field_0x1d4[3];
        param[5] = self->field_0x204;
        param[6] = self->field_0x228.x;
        param[7] = self->field_0x228.y;
        // An empty or inverted range falls back to channel 2's target.
        f32 range_end = self->field_0x21c;
        f32 range_start = self->field_0x210;
        if (range_end > range_start) {
            param[8] = range_start;
        } else {
            param[8] = self->field_0x4c.x;
            range_end = self->field_0x4c.y;
        }
        param[9] = range_end;
        fn_7100219294(g_unk_7101d47300, 0x8f2f84ad24e74e64, param, 0);
        self->field_0x270 = false;
    }
    if (!self->done[18])
        fn_7100f9aeec(dt, fade);
    if (!self->done[19]) {
        self->Advance(19, dt);
        self->field_0x288.value = FadeLerp(self->progress[19], self->field_0x288.from, self->field_0x288.to);
        self->field_0x2a0 = true;
    }
    if (!self->done[20]) {
        self->Advance(20, dt);
        self->field_0x294.value = FadeLerp(self->progress[20], self->field_0x294.from, self->field_0x294.to);
        self->field_0x2a0 = true;
    }
    if (self->field_0x2a0) {
        f32 param[2] = {self->field_0x288.value, self->field_0x294.value};
        fn_7100219294(g_unk_7101d47300, 0x5c4bf62fb5704af4, param, 0);
        self->field_0x2a0 = false;
    }
    if (!self->done[21]) {
        self->Advance(21, dt);
        self->field_0x2a4 = FadeLerp(self->progress[21], self->field_0x2a8.from, self->field_0x2a8.to);
        self->field_0x31c = true;
    }
    if (!self->done[22]) {
        self->Advance(22, dt);
        self->field_0x2b0 = FadeLerp(self->progress[22], self->field_0x2b4.from, self->field_0x2b4.to);
        self->field_0x31c = true;
    }
    if (!self->done[23]) {
        self->Advance(23, dt);
        self->field_0x2bc = FadeLerp(self->progress[23], self->field_0x2c0.from, self->field_0x2c0.to);
        self->field_0x31c = true;
    }
    if (!self->done[24]) {
        self->Advance(24, dt);
        self->field_0x2c8 = FadeLerp(self->progress[24], self->field_0x2cc.from, self->field_0x2cc.to);
        self->field_0x31c = true;
    }
    if (!self->done[25]) {
        self->Advance(25, dt);
        self->field_0x2d4 = FadeLerp(self->progress[25], self->field_0x2d8.from, self->field_0x2d8.to);
        self->field_0x31c = true;
    }
    if (!self->done[26])
        fn_7100f9b038(dt, fade);
    if (!self->done[27]) {
        self->Advance(27, dt);
        self->field_0x310 = FadeLerp(self->progress[27], self->field_0x314.from, self->field_0x314.to);
        self->field_0x31c = true;
    }
    if (self->field_0x31c) {
        f32 param[11];
        param[0] = self->field_0x2a1 ? 1.0f : 0.0f;
        param[1] = self->field_0x2a4;
        param[2] = self->field_0x2b0;
        param[3] = self->field_0x2bc;
        param[4] = self->field_0x2c8;
        param[5] = self->field_0x2d4;
        param[6] = self->field_0x2e0[0];
        param[7] = self->field_0x2e0[1];
        param[8] = self->field_0x2e0[2];
        param[9] = self->field_0x2e0[3];
        param[10] = self->field_0x310;
        fn_7100219294(g_unk_7101d47300, 0x46342169c9265aea, param, 0);
        self->field_0x31c = false;
    }
    if (self->field_0x344)
        fn_7100f9b170(dt, fade);
}
