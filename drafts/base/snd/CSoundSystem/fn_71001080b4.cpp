// base::snd::CSoundSystem's unit: plays a sound at most once per its group's interval and only while
// few sounds are pending; a sound of a group whose bit is set in the mask at +0x40091c gets 0.2 through
// fn_7100105bac. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU101.h"
#include "types.h"


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

// A handle on a playing sound: valid while the reference's serial is still the handle's.
struct UnkPlayingHandle_l1080b4 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;

    bool IsValid() const { return ref && ref->serial == serial && serial != ~0ull; }
    UnkPlayingSound_ovb0074* Get() const { return IsValid() ? ref->sound : nullptr; }
};

// What a throttled play request names: its group (or, when that is INT_MAX, the one at +0x8) and
// its path id at +0x20.
struct UnkSoundDesc_l1080b4 {
    s32 group;
    u8 pad_0x4[4];
    s32 fallback_group;  // 0x8
    u8 pad_0xc[0x20 - 0xc];
    base::global::CFilePathStrId path;  // 0x20

    s32 Group() const { return group == 0x7fffffff ? fallback_group : group; }
};

struct UnkFrameTimer_l1080b4 {
    u8 pad_0x0[0x2c];
    f32 time;  // 0x2c
};

struct UnkSoundSystemThrottle_l1080b4 {
    void** vtable;
    u8 member[0x4008b0 - 0x8];  // 0x8
    f32 last_played[8];         // 0x4008b0
    f32 interval[8];            // 0x4008d0
    u32 limits[8];              // 0x4008f0
    bool enabled[8];            // 0x400910
    u32 mask_a;                 // 0x400918
    u32 mask_b;                 // 0x40091c
};
static_assert(__builtin_offsetof(UnkSoundSystemThrottle_l1080b4, mask_b) == 0x40091c, "layout");

u32 fn_71000fd308(void* member);
UnkPlayingHandle_l1080b4 fn_71000fc98c(void* member, UnkSoundDesc_l1080b4* desc);

UnkPlayingHandle_l1080b4 fn_71001080b4(UnkSoundSystemThrottle_l1080b4* self, UnkSoundDesc_l1080b4* desc) {
    if (desc->path.data != &g_unk_7101d47200->empty) {
        s32 group = desc->Group();
        if (self->enabled[group]) {
            f32 now = reinterpret_cast<UnkFrameTimer_l1080b4*>(g_unk_7101d47328)->time;
            f32& last = self->last_played[group];
            if (now - last >= self->interval[group]) {
                last = now;
                if (fn_71000fd308(self->member) <= 4) {
                    UnkPlayingHandle_l1080b4 handle = fn_71000fc98c(self->member, desc);
                    const UnkPlayingHandle_l1080b4& empty =
                        reinterpret_cast<const UnkPlayingHandle_l1080b4&>(g_unk_7108b6c460);
                    if (handle.Get() != empty.Get() && (self->mask_b & (1u << group)))
                        fn_7100105bac(handle.Get(), 0.2f);
                    return handle;
                }
            }
        }
    }
    UnkPlayingHandle_l1080b4 none;
    none.ref = nullptr;
    none.serial = ~0ull;
    return none;
}
