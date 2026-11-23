// media::CAtkSoundStream's unit (tools/tu_map.py, unit 318): starts the stream. It starts the
// "AtkSoundStream Thread" (fn_710021b2ac), plays the stream's path as a group 1 sound, keeps the
// handle at +0x248 and sets the playing sound's value to 0. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkPlayingRef_ovb0074.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/global/timeline/CEvent.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/media/CMediaStream.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringEntry;

using UnkPathMember_l21b09c = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The file path of a string id's text (its use count at +0x44 goes up first).
struct UnkStreamPath_l21b09c {
    base::global::UnkStringData* data;

    explicit UnkStreamPath_l21b09c(const CStrId& id) {
        UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
        ++entry->tail.field_0x44;
        fn_710001a270(reinterpret_cast<base::global::UnkFilePathStrIdBase*>(this), entry->buffer, true);
    }
    ~UnkStreamPath_l21b09c() { reinterpret_cast<UnkPathMember_l21b09c*>(this)->~UnkPathMember_l21b09c(); }
    void operator=(const UnkPathMember_l21b09c& other) { *reinterpret_cast<UnkPathMember_l21b09c*>(this) = other; }
};

// A string id copied from another (retained) and released in two steps.
struct UnkStreamName_l21b09c {
    base::global::UnkStringData* data;

    explicit UnkStreamName_l21b09c(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkStreamName_l21b09c() {
        using base::UnkStrIdMember;
        reinterpret_cast<UnkStrIdMember*>(this)->~UnkStrIdMember();
    }
};

// What the sound system plays (the shape of UnkSoundDesc_ovb0074).
struct UnkStreamDesc_l21b09c {
    s32 group;
    UnkStreamPath_l21b09c path;  // 0x8
    UnkStreamName_l21b09c name;  // 0x10
    s32 direction;               // 0x18
    u32 field_0x1c;
    u32 field_0x20;
    u32 variant;                 // 0x24
    u32 field_0x28;
    f32 volume;                  // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;

    UnkStreamDesc_l21b09c()
        : group(0), path(base::global::CStrId::Empty()), name(base::global::CStrId::Empty()),
          direction(1), field_0x1c(0), field_0x20(0xffffffff), variant(0), volume(1.0f), field_0x30(0.0f),
          field_0x34(0.0f), field_0x38(-1.0f) {}
};

struct UnkSoundSystemPlayView_ovb0074;
struct UnkSoundDesc_ovb0074;
struct UnkPlayingSound_ovb0074;
struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;
};
struct UnkSoundMgr;

struct UnkAtkSoundStream_ovb0008;
void fn_710021b2ac(UnkAtkSoundStream_ovb0008* self);

struct UnkAtkStreamSource_l21b09c {
    u8 pad_0x0[0x20];
    UnkPathMember_l21b09c path;  // 0x20
};

struct UnkAtkSoundStream_l21b09c {
    u8 pad_0x0[0x38];
    UnkAtkStreamSource_l21b09c* source;  // 0x38
    u8 pad_0x40[0x10];
    u8 thread[0x1f8];                    // 0x50
    UnkPlayingHandle_ovb0074 handle;     // 0x248
};

// The sound a handle names, or null once it is stale.
inline UnkPlayingSound_ovb0074* PlayingSound_l21b09c(const UnkPlayingHandle_ovb0074& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != ~0ull)
        return handle.ref->sound;
    return nullptr;
}

struct UnkMovieStreamNx_ovb0003;

void fn_710021b09c(UnkAtkSoundStream_l21b09c* self) {
    fn_710021b7c4(reinterpret_cast<UnkMovieStreamNx_ovb0003*>(self));
    fn_7100066744(self->thread, "AtkSoundStream Thread", reinterpret_cast<void (*)(void*)>(fn_710021b2ac), 0, 0x10,
                  self);
    UnkStreamDesc_l21b09c desc;
    desc.group = 1;
    desc.path = self->source->path;
    desc.direction = -1;
    self->handle = fn_7100108620(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820),
                                 reinterpret_cast<UnkSoundDesc_ovb0074*>(&desc));
    fn_7100105bac(PlayingSound_l21b09c(self->handle), 0.0f);
}
