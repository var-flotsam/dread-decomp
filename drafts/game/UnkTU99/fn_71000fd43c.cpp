// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): fn_71000fd43c starts a sound from its
// description: the file (its path sanitised by fn_71000f63b0) must be loaded; the path's playing voice is
// reused while it is live and not stopping, else a free voice is taken; a muted group only flags the voice,
// otherwise the voice is set up from the description. The handle of the voice's slot, or the empty handle.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"
#include "game/operator_new.h"
#include "game/sound/UnkSoundHandle.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

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

struct UnkSoundDesc_ovb0074;
struct UnkSoundSystemVoices_b56;
struct UnkSoundVoice2_b56;
struct UnkVoicePool_b56;
struct UnkVoiceStats_b56;
struct UnkVoiceStat_b56 {
    u64 field_0x0;
    u64 released;
};
UnkVoiceStat_b56& fn_7100103da0(UnkVoiceStats_b56* self, u32 index);
UnkSoundVoice2_b56* fn_71000fd89c(UnkSoundSystemVoices_b56* self);
void fn_71001059f0(UnkInterp_7100105d34* self, f32 time);
UnkPlayingHandle_ovb0074 fn_71000fd778(void* member, const CFilePathStrId* path);
extern const f32 g_unk_71016479f8[] __attribute__((visibility("hidden")));  // level values

// What a sound is started from.
struct UnkSoundStartDesc_l0fd43c {
    s32 group;  // 0x0
    u8 pad_0x4[4];
    CFilePathStrId path;  // 0x8
    CStrId name;          // 0x10
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 variant;  // 0x24
    u32 field_0x28;
    f32 volume;      // 0x2c
    f32 field_0x30;  // 0x30
    u32 field_0x34;
    u32 field_0x38;
};

// A voice as this sets it up.
struct UnkSoundVoice_l0fd43c {
    f32 volume[3];  // 0x0
    u8 pad_0xc[0x88 - 0xc];
    CStrId name;          // 0x88
    CFilePathStrId path;  // 0x90
    u8 pad_0x98[0xb0 - 0x98];
    s32 group;  // 0xb0
    u8 pad_0xb4[0xcc - 0xb4];
    u32 field_0xcc;
    u32 field_0xd0;
    u32 field_0xd4;
    u32 variant;  // 0xd8
    u32 field_0xdc;
    u8 pad_0xe0[0xec - 0xe0];
    f32 field_0xec;
    u8 pad_0xf0[0xfc - 0xf0];
    f32 field_0xfc;
    u32 field_0x100;
    u8 pad_0x104[0x110 - 0x104];
    u32 field_0x110;
    u8 pad_0x114[0x119 - 0x114];
    bool muted;  // 0x119
    u8 pad_0x11a[2];
    bool field_0x11c;
    u8 pad_0x11d[3];
    bool field_0x120;
    bool field_0x121;
    bool field_0x122;
};
static_assert(__builtin_offsetof(UnkSoundVoice_l0fd43c, field_0x122) == 0x122, "layout");

// A loaded file of the sound system's map: its key and the next node of its bucket's chain.
struct UnkSoundFileNode_l0fd43c {
    u64 key;  // 0x0
    u8 pad_0x8[0x18];
    UnkSoundFileNode_l0fd43c* next;  // 0x20
};

struct UnkSoundSystem_l0fd43c {
    u8 pad_0x0[0xe8];
    u32 muted_groups;  // 0xe8
    u8 pad_0xec[0x348 - 0xec];
    u32 level_mask;  // 0x348
    s32 levels[8];   // 0x34c
    u8 pad_0x36c[0x398 - 0x36c];
    u8 pool[0x28];  // 0x398
    u8 stats[8];    // 0x3c0
    u8 pad_0x3c8[0x4c8 - 0x3c8];
    UnkSoundFileNode_l0fd43c** buckets;  // 0x4c8
    u32 used;                            // 0x4d0
    u32 bucket_count;                    // 0x4d4
    u8 pad_0x4d8[0x400568 - 0x4d8];
    nn::os::MutexType mutex;  // 0x400568
};
static_assert(__builtin_offsetof(UnkSoundSystem_l0fd43c, mutex) == 0x400568, "layout");

// The file's key: the CRC-64 of its pooled text.
struct UnkPathData_l0fd43c {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

// CStrId's copy-assignment, on the file-path pool.
static inline void AssignPath(CFilePathStrId& dst, const CFilePathStrId& src) {
    if (dst.data != src.data) {
        UnkStringData* d = src.data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (dst.data != &pool->empty) {
            --dst.data->ref_count;
            if (dst.data->ref_count == 0)
                StringPool_Release(pool, dst.data);
        }
        dst.data = d;
    }
}

static inline UnkPlayingHandle_ovb0074 EmptyHandle() {
    return {reinterpret_cast<UnkPlayingRef_ovb0074*>(g_unk_7108b6c460.ref), static_cast<u64>(g_unk_7108b6c460.serial)};
}

UnkPlayingHandle_ovb0074 fn_71000fd43c(void* member, UnkSoundDesc_ovb0074* description) {
    UnkSoundSystem_l0fd43c* self = static_cast<UnkSoundSystem_l0fd43c*>(member);
    UnkSoundStartDesc_l0fd43c* desc = reinterpret_cast<UnkSoundStartDesc_l0fd43c*>(description);
    if (g_unk_7108b6ab60 == 1)
        return EmptyHandle();
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    UnkPlayingHandle_ovb0074 result;
    {
        CFilePathStrId id = fn_71000f63b0(desc->path);
        bool loaded = false;
        if (self->used) {
            u64 key = reinterpret_cast<UnkPathData_l0fd43c*>(id.data)->key;
            UnkSoundFileNode_l0fd43c** bucket = static_cast<u32>(key) % self->bucket_count + self->buckets;
            if (bucket) {
                for (UnkSoundFileNode_l0fd43c* node = *bucket; node; node = node->next) {
                    if (node->key == key) {
                        loaded = true;
                        break;
                    }
                }
            }
        }
        UnkSoundVoice_l0fd43c* voice = nullptr;
        if (loaded) {
            voice = reinterpret_cast<UnkSoundVoice_l0fd43c*>(fn_71000fd778(self, &desc->path).Get());
            if (!voice || voice->field_0x122 || !voice->field_0x121)
                voice = reinterpret_cast<UnkSoundVoice_l0fd43c*>(
                    fn_71000fd89c(reinterpret_cast<UnkSoundSystemVoices_b56*>(self)));
        }
        if (voice) {
            if (self->muted_groups & (1u << desc->group)) {
                voice->muted = true;
            } else {
                AssignPath(voice->path, id);
                voice->name = desc->name;
                voice->field_0x120 = true;
                voice->group = desc->group;
                voice->field_0x11c = desc->field_0x18 > 1;
                voice->field_0xd4 = desc->field_0x18;
                voice->field_0xcc = desc->field_0x1c;
                voice->field_0xd0 = desc->field_0x20;
                voice->variant = desc->variant;
                voice->field_0xdc = desc->field_0x28;
                voice->field_0xec = desc->volume;
                voice->volume[1] = desc->volume;
                voice->volume[2] = desc->volume;
                voice->volume[0] = desc->volume;
                voice->field_0xfc = desc->field_0x30;
                voice->field_0x100 = desc->field_0x34;
                voice->field_0x110 = desc->field_0x38;
                fn_71001059f0(reinterpret_cast<UnkInterp_7100105d34*>(voice), desc->field_0x30);
                voice->muted = false;
                s32 group = desc->group;
                if (self->level_mask & (1u << group))
                    fn_7100105ef4(voice, g_unk_71016479f8[self->levels[group]], 0.0f, true);
            }
            u32 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(self->pool),
                                      reinterpret_cast<UnkSoundVoice2_b56*>(voice));
            UnkVoiceStat_b56& stat = fn_7100103da0(reinterpret_cast<UnkVoiceStats_b56*>(self->stats), index);
            result = {reinterpret_cast<UnkPlayingRef_ovb0074*>(&stat), stat.released};
        } else {
            result = EmptyHandle();
        }
    }
    fn_7100066920(mutex);
    return result;
}
