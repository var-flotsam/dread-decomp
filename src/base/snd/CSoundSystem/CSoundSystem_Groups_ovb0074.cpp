// base::snd::CSoundSystem's unit (tools/tu_map.py, the name from reflection). The group flags at
// +0x400910 (fn_7100107eb8: a flag turned off also clears its group in the member at +0x8, and each one
// is mirrored into the global settings g_unk_7108b6c5f0 from +0x21), the two group masks at +0x400918
// and +0x40091c (fn_71001082a8, fn_7100108464), and two ways of playing a sound under the system's
// mutex at +0x400920 (fn_7100108620, fn_71001088d4). The temporaries handed to the member by address
// sit in 8-byte slots in the original (alignas). Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkSoundSettings_ovb0074 {
    u8 pad_0x0[0x21];
    bool flags[8];  // 0x21
};
extern UnkSoundSettings_ovb0074 g_unk_7108b6c5f0;

void fn_71000fea74(void* member, u32 mask, f32 fade);
void fn_71000fd1d4(void* member, const u32* mask, f32 fade);

struct UnkSoundSystemFlags_ovb0074 {
    void** vtable;
    u8 member[0x400910 - 0x8];  // 0x8
    bool flags[8];              // 0x400910
};

void fn_7100107eb8(UnkSoundSystemFlags_ovb0074* self, bool on, s32 index) {
    bool& flag = self->flags[index];
    if (flag == on)
        return;
    flag = on;
    if (!on) {
        void* member = self->member;
        u32 mask = 1u << index;
        fn_71000fea74(member, mask, 0.0f);
        alignas(8) u32 bits = mask;
        fn_71000fd1d4(member, &bits, 0.0f);
    }
    switch (index) {
    case 0:
        g_unk_7108b6c5f0.flags[0] = on;
        break;
    case 1:
        g_unk_7108b6c5f0.flags[1] = on;
        break;
    case 2:
        g_unk_7108b6c5f0.flags[2] = on;
        break;
    case 3:
        g_unk_7108b6c5f0.flags[3] = on;
        break;
    case 4:
        g_unk_7108b6c5f0.flags[4] = on;
        break;
    case 5:
        g_unk_7108b6c5f0.flags[5] = on;
        break;
    case 6:
        g_unk_7108b6c5f0.flags[6] = on;
        break;
    case 7:
        g_unk_7108b6c5f0.flags[7] = on;
        break;
    }
}

void fn_71000fd0cc(void* member, bool on, const u32* mask);
struct UnkSoundSystemATK;
void fn_71000fbce0(UnkSoundSystemATK* self, bool on, const s32* mask);

struct UnkSoundSystemMasks_ovb0074 {
    void** vtable;
    u8 member[0x400918 - 0x8];  // 0x8
    u32 mask_a;                 // 0x400918
    u32 mask_b;                 // 0x40091c
};

__attribute__((always_inline)) static inline void ApplyMask(u32* bits, bool on, const u32* mask) {
    if (on) {
        for (u32 i = 0; i < 8; ++i)
            if (*mask & (1u << i))
                *bits |= 1u << i;
    } else {
        for (u32 i = 0; i < 8; ++i)
            if (*mask & (1u << i))
                *bits &= ~(1u << i);
    }
}

void fn_71001082a8(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask) {
    ApplyMask(&self->mask_a, on, mask);
    alignas(8) u32 groups = *mask;
    fn_71000fd0cc(self->member, on, &groups);
}

void fn_7100108464(UnkSoundSystemMasks_ovb0074* self, bool on, const u32* mask) {
    ApplyMask(&self->mask_b, on, mask);
    alignas(8) s32 groups = *mask;
    fn_71000fbce0(reinterpret_cast<UnkSoundSystemATK*>(self->member), on, &groups);
}

struct UnkPlayingSound_ovb0074 {
    u8 pad_0x0[0xd8];
    u32 variant;  // 0xd8
    u8 pad_0xdc[0x11b - 0xdc];
    bool field_0x11b;
};

struct UnkPlayingRef_ovb0074 {
    UnkPlayingSound_ovb0074* sound;
    u64 serial;
};

struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;

    bool IsValid() const { return ref && serial != ~0ull && ref->serial == serial; }
    UnkPlayingSound_ovb0074* Get() const { return IsValid() ? ref->sound : nullptr; }
};

struct UnkSoundDesc_ovb0074 {
    s32 group;
    u8 pad_0x4[4];
    UnkStringData* path;  // 0x8, a CFilePathStrId
    u8 pad_0x10[0x24 - 0x10];
    u32 variant;  // 0x24
    u8 pad_0x28[4];
    f32 field_0x2c;
    f32 field_0x30;
};

struct UnkSoundSystemPlayView_ovb0074 {
    void** vtable;
    u8 member[0x4008f0 - 0x8];  // 0x8
    u32 limits[8];              // 0x4008f0
    bool enabled[8];            // 0x400910
    u8 pad_0x400918[8];
    nn::os::MutexType mutex;  // 0x400920
};

UnkPlayingHandle_ovb0074 fn_71000fe418(void* member, UnkStringData** path, u32 flags);
void fn_71000fd9f8(void* member, UnkPlayingRef_ovb0074* ref, u64 serial, UnkSoundDesc_ovb0074* desc);
void fn_7100105034(void*);
u32 fn_71000fef18(void* member, u32 mask);
UnkPlayingHandle_ovb0074 fn_71000fd43c(void* member, UnkSoundDesc_ovb0074* desc);
struct UnkInterp_7100105d34;
void fn_7100105d34(UnkInterp_7100105d34*, f32, bool, f32);
void fn_7100105c00(UnkPlayingSound_ovb0074* sound, f32 value);
UnkPlayingHandle_ovb0074 fn_71000fdb1c(void* member, UnkSoundDesc_ovb0074* desc, void* arg);
bool fn_71001087a8(UnkSoundSystemPlayView_ovb0074* self, u32 group);

// Holds the system's mutex for its scope.
struct UnkSoundLock_ovb0074 {
    nn::os::MutexType* mutex;
    explicit UnkSoundLock_ovb0074(nn::os::MutexType* m) : mutex(m) { fn_710006691c(mutex); }
    ~UnkSoundLock_ovb0074() { fn_7100066920(mutex); }
};

// Plays a sound described by `desc` (its group at +0x0, its path id at +0x8). Nothing happens for an
// empty path or a group whose flag is off. The sound already playing for the path (fn_71000fe418) is
// restarted (fn_71000fd9f8) when its variant (+0xd8) is the requested one (+0x24), or stopped
// (fn_7100105034); a new one is started (fn_71000fd43c) while the group has fewer sounds
// (fn_71000fef18) than its limit (+0x4008f0), or once the group's finished sounds were stopped
// (fn_71001087a8). Returns the handle.
UnkPlayingHandle_ovb0074 fn_7100108620(UnkSoundSystemPlayView_ovb0074* self, UnkSoundDesc_ovb0074* desc) {
    UnkPlayingHandle_ovb0074 handle;
    if (desc->path == &g_unk_7101d47200->empty || !self->enabled[desc->group]) {
        handle.ref = nullptr;
        handle.serial = ~0ull;
        return handle;
    }
    UnkSoundLock_ovb0074 lock(&self->mutex);
    handle = fn_71000fe418(self->member, &desc->path, 0);
    if (handle.IsValid() && handle.ref->sound) {
        if (handle.ref->sound->variant == desc->variant) {
            fn_71000fd9f8(self->member, handle.ref, handle.serial, desc);
            return handle;
        }
        fn_7100105034(handle.ref->sound);
    }
    if (fn_71000fef18(self->member, 1u << desc->group) < self->limits[desc->group])
        handle = fn_71000fd43c(self->member, desc);
    if (!(handle.IsValid() && handle.ref->sound)) {
        if (fn_71001087a8(self, desc->group))
            handle = fn_71000fd43c(self->member, desc);
    }
    return handle;
}

// The same with an extra argument for a new sound (fn_71000fdb1c): a sound already playing for the
// path is updated instead (fn_7100105d34 with +0x2c and +0x30, then fn_7100105c00 with +0x30 when its
// flag at +0x11b is set) and kept.
UnkPlayingHandle_ovb0074 fn_71001088d4(UnkSoundSystemPlayView_ovb0074* self, UnkSoundDesc_ovb0074* desc, void* arg) {
    UnkPlayingHandle_ovb0074 handle;
    if (desc->path == &g_unk_7101d47200->empty || !self->enabled[desc->group]) {
        handle.ref = nullptr;
        handle.serial = ~0ull;
        return handle;
    }
    UnkSoundLock_ovb0074 lock(&self->mutex);
    handle = fn_71000fe418(self->member, &desc->path, 0);
    if (handle.IsValid() && handle.ref->sound) {
        fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(handle.ref->sound), desc->field_0x2c, false,
                      desc->field_0x30);
        if (handle.ref->sound->field_0x11b)
            fn_7100105c00(handle.Get(), desc->field_0x30);
        return handle;
    }
    if (fn_71000fef18(self->member, 1u << desc->group) < self->limits[desc->group])
        handle = fn_71000fdb1c(self->member, desc, arg);
    if (!(handle.IsValid() && handle.ref->sound)) {
        if (fn_71001087a8(self, desc->group))
            handle = fn_71000fdb1c(self->member, desc, arg);
    }
    return handle;
}
