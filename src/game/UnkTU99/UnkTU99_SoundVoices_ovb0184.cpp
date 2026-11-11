// Unit 99 (the sound system's, reflected as base::snd::CSoundSystemATK::CTunableSoundSystemATK): the
// setters that apply a value to every voice of one of the sound system's two voice lists whose group
// bit is in a mask (fn_71000fea74 and its siblings), the effect-slot setters (fn_7100100734,
// fn_71000fb9f4), a value fading towards a target (fn_71000f5260) and the frame-time
// accumulator (fn_71000f6770). Names and layouts are placeholders.
#include <nn/os.h>
#include <nn/util.h>

#include "base/UnkDefaultAllocator.h"
#include "base/snd/UnkSound.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2772.h"
#include "game/UnkTU58.h"

using base::snd::UnkSoundSystemATK;

extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));

struct UnkInterp_7100105d34;
struct UnkPlayingSound_ovb0074;
void fn_7100105bac(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105c9c(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105cf0(UnkPlayingSound_ovb0074* sound, f32 value);

// A voice: the group bit it answers to is 1 << shift.
struct UnkVoice_b0184 {
    u8 pad_0x0[0xb0];
    u32 shift;  // 0xb0
};

// The members of the sound system these functions touch: two lists of voices and the mutex that
// guards them.
struct UnkSystem_b0184 {
    u8 pad_0x0[0xe8];
    u32 field_0xe8;
    u8 pad_0xec[0x3f0 - 0xec];
    UnkVoice_b0184** list_0x3f0;
    u32 count_0x3f8;
    u8 pad_0x3fc[0x1c];
    UnkVoice_b0184** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;  // 0x400568
};

// Applies a fade (value, no limit) to the second list's voices in `mask`.
void fn_71000fea74(void* member, u32 mask, f32 fade) {
    UnkSystem_b0184* self = reinterpret_cast<UnkSystem_b0184*>(member);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (UnkVoice_b0184 **it = self->list_0x418, **end = it + self->count_0x420; it != end; ++it) {
        u32 shift = (*it)->shift;
        if ((1u << shift) & mask)
            fn_7100105a80(reinterpret_cast<UnkSound*>(*it), fade, 0.0f);
    }
    fn_7100066920(&self->mutex);
}

void fn_71000fe9a0(UnkSystem_b0184* self, u32 mask, f32 first, f32 second, bool flag) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (UnkVoice_b0184 **it = self->list_0x418, **end = it + self->count_0x420; it != end; ++it) {
        if ((1u << (*it)->shift) & mask)
            fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(*it), second, flag, first);
    }
    fn_7100066920(&self->mutex);
}

UnkVoice_b0184** fn_710010039c(void* vector, u32 index);

void fn_71001002c0(UnkSystem_b0184* self, const u32* mask, f32 value) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (u32 i = 0; i < self->count_0x3f8; ++i) {
        UnkVoice_b0184* voice = *fn_710010039c(&self->list_0x3f0, i);
        u32 shift = voice->shift;
        if (*mask & (1u << shift))
            fn_7100105a80(reinterpret_cast<UnkSound*>(voice), value, 0.0f);
    }
    fn_7100066920(&self->mutex);
}

void fn_71000feb3c(UnkSystem_b0184* self, u32 mask, bool flag, f32 value) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (UnkVoice_b0184 **it = self->list_0x418, **end = it + self->count_0x420; it != end; ++it) {
        if ((1u << (*it)->shift) & mask) {
            if (flag)
                fn_7100105bac(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
            else
                fn_7100105c00(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
        }
    }
    fn_7100066920(&self->mutex);
}

void fn_71000fbce0(UnkSoundSystemATK* sys, bool on, const s32* mask) {
    UnkSystem_b0184* self = reinterpret_cast<UnkSystem_b0184*>(sys);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (UnkVoice_b0184 **it = self->list_0x3f0, **end = it + self->count_0x3f8; it != end; ++it) {
        u32 shift = (*it)->shift;
        if (*mask & (1u << shift)) {
            if (on)
                fn_7100105bac(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), 0.2f);
            else
                fn_7100105c00(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), 0.2f);
        }
    }
    fn_7100066920(&self->mutex);
}

void fn_71000fd0cc(void* member, bool on, const u32* mask) {
    UnkSystem_b0184* self = reinterpret_cast<UnkSystem_b0184*>(member);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (UnkVoice_b0184 **it = self->list_0x3f0, **end = it + self->count_0x3f8; it != end; ++it) {
        u32 shift = (*it)->shift;
        if (*mask & (1u << shift)) {
            if (on)
                fn_7100105c9c(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), 0.2f);
            else
                fn_7100105cf0(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), 0.2f);
        }
    }
    fn_7100066920(&self->mutex);
}

// Stores `flag ? mask : 0` at +0xe8, then applies a value to the second list's voices in `mask`.
void fn_71000fe894(UnkSystem_b0184* self, u32 mask, bool flag, f32 value) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    self->field_0xe8 = flag ? mask : 0;
    for (UnkVoice_b0184 **it = self->list_0x418, **end = it + self->count_0x420; it != end; ++it) {
        if ((1u << (*it)->shift) & mask) {
            if (flag)
                fn_7100105c9c(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
            else
                fn_7100105cf0(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
        }
    }
    fn_7100066920(&self->mutex);
}

// ---- the effect slots ----

extern u8 g_unk_7108b6b600[] __attribute__((visibility("hidden")));
void* fn_710120806c(void* player, u32 id);
void fn_7101209ce0(void* effect, f32 value);
void fn_7101209ce8(void* effect, s32 index, f32 value);

// Resets the effect of sound `id`: kind 0 also clears the index, 1 and 7 only the first value, 2-5 and
// 8-12 set the index to kind - 1 or kind - 7; nothing for the others.
void fn_7100100734(void* self, u32 id, u32 kind) {
    nn::os::GetCurrentThread();
    void* effect = fn_710120806c(g_unk_7108b6b600, id + 0x4000000);
    switch (kind) {
    case 0:
        fn_7101209ce0(effect, 0.0f);
        fn_7101209ce8(effect, -1, 0.0f);
        break;
    case 1:
    case 7:
        fn_7101209ce0(effect, 0.0f);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        fn_7101209ce8(effect, kind - 1, 0.0f);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        fn_7101209ce8(effect, kind - 7, 0.0f);
        break;
    }
}

bool fn_7101234580(void* effect);
void fn_7101234588(void* effect, u32 value);
bool fn_7101233f24(void* effect);
void fn_7101233f2c(void* effect, u32 value);
bool fn_7101234dc4(void* effect);
void fn_7101234dcc(void* effect, u32 value);
extern u8 g_unk_7108b6ae38[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6afe8[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6adb0[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6af50[] __attribute__((visibility("hidden")));

struct UnkTunableFlags_b0184 {
    u8 pad_0x0[0x36c];
    u8 field_0x36c;
};

// Reads one of the four effects' changed flag into +0x36c, then resets it.
void fn_71000fb9f4(UnkTunableFlags_b0184* self, u32 kind) {
    nn::os::GetCurrentThread();
    switch (kind) {
    case 0:
        return;
    case 1:
        self->field_0x36c |= fn_7101234dc4(g_unk_7108b6af50);
        fn_7101234dcc(g_unk_7108b6af50, 0);
        return;
    case 2:
        self->field_0x36c |= fn_7101234580(g_unk_7108b6afe8);
        fn_7101234588(g_unk_7108b6afe8, 0);
        return;
    case 3:
        self->field_0x36c |= fn_7101233f24(g_unk_7108b6adb0);
        fn_7101233f2c(g_unk_7108b6adb0, 0);
        return;
    case 4:
        self->field_0x36c |= fn_7101233f24(g_unk_7108b6ae38);
        fn_7101233f2c(g_unk_7108b6ae38, 0);
        return;
    default:
        NN_UNEXPECTED_DEFAULT;
    }
}

// ---- a value fading towards a target ----

class UnkEffect_b0184 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28();
};

struct UnkEffectRef_b0184 {
    UnkEffect_b0184* effect;
};

struct UnkOwner_b0184 {
    u8 pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    UnkEffectRef_b0184* ref;  // 0x28
};

void fn_71000f5260(UnkOwner_b0184* self, f32 value, bool enable, f32 time) {
    if (!self->ref)
        return;
    f32 target = enable ? value : 0.0f;
    if (time <= 0.0f && self->ref->effect && self->ref->effect->vf_0x28()) {
        if (!self->ref->effect)
            return;
        fn_710120e660(self->ref->effect, target, 0);
        if (!self->ref->effect)
            return;
        fn_710120e660(self->ref->effect, 0.0f, 1);
        if (!self->ref->effect)
            return;
        fn_710120e660(self->ref->effect, 0.0f, 2);
        if (!self->ref->effect)
            return;
        fn_710120e658(self->ref->effect, -target);
        return;
    }
    f32 limit = time > 0.1f ? time : 0.1f;
    self->field_0x18 = self->field_0x14;
    self->field_0x1c = target;
    self->field_0x20 = limit;
    self->field_0x24 = 0.0f;
}

// ---- the frame-time accumulator ----

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

struct UnkIdMap;
struct UnkSoundTimeSystem_b0184 {
    u8 pad_0x0[0x37c];
    f32 time;  // 0x37c
    u8 pad_0x380[0x400528 - 0x380];
    nn::os::MutexType mutex;  // 0x400528
};
extern UnkIdMap* g_unk_7101d685c8;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

bool fn_7100132a90();

// The function-local static (the time of the last call) and its guard, then the time of this call.
struct UnkTimeStatic_b0184 {
    s64 last;
    u64 guard;
    s64 now;
};
extern UnkTimeStatic_b0184 g_unk_7108b6ab80 __attribute__((visibility("hidden")));

// Adds the microseconds since the last call, in seconds, to the sound system's clock at +0x37c.
void fn_71000f6770() {
    if (fn_7100132a90())
        return;
    UnkTimeStatic_b0184* state = &g_unk_7108b6ab80;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&state->guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&state->guard)) {
            state->last = fn_7100080b7c(g_unk_7101d47328);
            __cxa_guard_release(&state->guard);
        }
    }
    state->now = fn_7100080b7c(g_unk_7101d47328);
    UnkSoundTimeSystem_b0184* system = reinterpret_cast<UnkSoundTimeSystem_b0184*>(g_unk_7101d685c8);
    if (!system)
        return;
    f64 seconds = (f64)(state->now - state->last) * 0.000001;
    nn::os::MutexType* mutex = &system->mutex;
    fn_710006691c(mutex);
    f32 time = system->time;
    fn_7100066920(mutex);
    time += (f32)seconds;
    fn_710006691c(mutex);
    system->time = time;
    fn_7100066920(mutex);
    state->last = state->now;
}
