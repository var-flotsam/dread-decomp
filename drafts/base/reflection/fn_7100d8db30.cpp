// Unit 2525 (base::reflection): the environment manager's sound setter. The new environment sound
// replaces the current one (+0x480): the same stream (unless forced) only fades to the new levels,
// another one stops the current sound and, unless paused, plays the new stream; then its kind is
// muted or unmuted. With none, the current sound stops and every channel is unmuted. The name of the
// sound is kept at +0x478. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkMask.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/reflection.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringEntry;

using UnkPathMember_ovb0138 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The file path of a string id's text (its use count at +0x44 goes up first).
struct UnkStreamPath_ovb0138 {
    base::global::UnkStringData* data;

    explicit UnkStreamPath_ovb0138(const CStrId& id) {
        UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
        ++entry->tail.field_0x44;
        fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(this), entry->buffer, true);
    }
    ~UnkStreamPath_ovb0138() { reinterpret_cast<UnkPathMember_ovb0138*>(this)->~UnkPathMember_ovb0138(); }
    void operator=(const UnkPathMember_ovb0138& other) { *reinterpret_cast<UnkPathMember_ovb0138*>(this) = other; }
};

// A string id copied from another (retained) and released in two steps.
struct UnkStreamName_ovb0138 {
    base::global::UnkStringData* data;

    explicit UnkStreamName_ovb0138(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkStreamName_ovb0138() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

// What the sound system plays (UnkStreamDesc_ovb0138's shape), kept in a static here; its
// destructor is out of line.
struct UnkEnvStreamDesc_d8db30 {
    s32 group;
    UnkStreamPath_ovb0138 path;  // 0x8
    UnkStreamName_ovb0138 name;  // 0x10
    s32 direction;               // 0x18
    u32 field_0x1c;
    u32 field_0x20;
    u32 variant;                 // 0x24
    u32 field_0x28;
    f32 volume;                  // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;

    UnkEnvStreamDesc_d8db30()
        : group(0), path(g_unk_7101d470e0), name(g_unk_7101d470e0),
          direction(1), field_0x1c(0), field_0x20(0xffffffff), variant(0), volume(1.0f), field_0x30(0.0f),
          field_0x34(0.0f), field_0x38(-1.0f) {}
    ~UnkEnvStreamDesc_d8db30();  // fn_7100d8ef0c
};

struct UnkSoundSystemPlayView_ovb0074;
struct UnkSoundDesc_ovb0074;
struct UnkPlayingRef_ovb0074;
struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;
};

struct UnkSoundHandle_b46a1;
void fn_7100108b90(char* self, u32 id, const UnkSoundHandle_b46a1& handle, f32 value);
void fn_7100108be8(UnkSoundMgr* self, const u32* mask, f32 time);

// The environment sound: its name, stream path, levels and the sound kind it mutes.
struct UnkEnvSound_d8db30 {
    CStrId name;                // 0x0
    UnkPathMember_ovb0138 path;  // 0x8
    u8 unk_0x10[0x8];
    f32 field_0x18;
    f32 fade_time;  // 0x1c
    f32 field_0x20;
    f32 field_0x24;
    u32 kind;  // 0x28
};
static_assert(__builtin_offsetof(UnkEnvSound_d8db30, kind) == 0x28, "layout");

// The environment manager as the setter sees it.
struct UnkEnvManager_d8db30 {
    u8 unk_0x0[0x478];
    CStrId sound_name;            // 0x478
    UnkEnvSound_d8db30* sound;    // 0x480
    u8 unk_0x488[0x4a2 - 0x488];
    bool sound_paused;  // 0x4a2
    bool muting;        // 0x4a3
    u8 unk_0x4a4[0x4c2 - 0x4a4];
    bool mute_blocked;  // 0x4c2
};
static_assert(__builtin_offsetof(UnkEnvManager_d8db30, mute_blocked) == 0x4c2, "layout");

// The sound channels: none, and all eight.
inline const UnkMask& UnkEnvChannelsNone_d8db30() {
    static UnkMask s_none asm("g_unk_7108b6ab30");
    return s_none;
}
inline const UnkMask& UnkEnvChannelsAll_d8db30() {
    static UnkMask s_all asm("g_unk_7108b6ab50")(~UnkEnvChannelsNone_d8db30().bits & 0xff);
    return s_all;
}

// Stops the current sound (fading it out); with none, the sound kind 5.
static inline void StopCurrent(UnkEnvSound_d8db30* current) {
    if (current) {
        if (current->path.data == g_unk_7101d470e8.data)
            return;
        fn_7100108abc(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), reinterpret_cast<UnkHandle*>(&current->path),
                      current->fade_time, 0.0f);
        return;
    }
    fn_7100108ae8(g_unk_7101d4d820, 5, 3.0f, 0.0f, false);
}

void fn_7100d8db30(UnkUnit2525_b51* manager, void* resource, bool force) {
    UnkEnvManager_d8db30* self = reinterpret_cast<UnkEnvManager_d8db30*>(manager);
    UnkEnvSound_d8db30* sound = static_cast<UnkEnvSound_d8db30*>(resource);
    UnkEnvSound_d8db30* current = self->sound;
    if (!sound) {
        StopCurrent(current);
        if (self->muting && !self->mute_blocked) {
            UnkSoundMgr* sounds = g_unk_7101d4d820;
            alignas(8) u32 channels = UnkEnvChannelsAll_d8db30().bits;
            fn_7100108be8(sounds, &channels, 0.0f);
        }
        self->sound = nullptr;
        self->sound_name = g_unk_7101d470e0;
        return;
    }

    if (current && current->path == sound->path && !force) {
        fn_7100108abc(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), reinterpret_cast<UnkHandle*>(&sound->path),
                      sound->field_0x18, sound->field_0x24);
    } else {
        StopCurrent(current);
        if (self->sound_paused && sound->path.data != &g_unk_7101d47200->empty) {
            static UnkEnvStreamDesc_d8db30 s_desc asm("g_unk_71727b8d98");
            s_desc.volume = sound->field_0x24;
            s_desc.direction = -1;
            s_desc.field_0x30 = sound->field_0x18;
            s_desc.field_0x34 = sound->field_0x20;
            s_desc.group = 5;
            s_desc.path = sound->path;
            fn_7100108620(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820),
                          reinterpret_cast<UnkSoundDesc_ovb0074*>(&s_desc));
        }
    }
    if (self->muting && !self->mute_blocked) {
        u64 channel = 1;
        fn_7100108b90(reinterpret_cast<char*>(g_unk_7101d4d820), sound->kind,
                      *reinterpret_cast<const UnkSoundHandle_b46a1*>(&channel), 0.0f);
    }
    self->sound = sound;
    self->sound_name = sound->name;
}
