// sound::CSoundManager's unit (tools/tu_map.py unit 2646): whether a preset may start another sound. Its group
// of playing sounds (keyed by the preset's +0x58) must all be at least the preset's minimum (+0x57c) loud;
// when the group already holds the preset's limit (+0x578), the loudest is faded out and dropped
// (fn_710102576c) to make room. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/snd/UnkSound.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CRntVector;

struct UnkSoundRef {
    void* object;
    s64 serial;
};

struct UnkDropTarget;
struct UnkAudioPreset_b0038;

// The playing sound a handle refers to, as far as this function reads it.
struct UnkPlayingSound_l02423c {
    u8 pad_0x0[0x108];
    f32 volume;  // 0x108
};

struct UnkPresetLimits_l02423c {
    u8 pad_0x0[0x58];
    u64 key;  // 0x58
    u8 pad_0x60[0x578 - 0x60];
    s32 max_sounds;  // 0x578
    f32 min_volume;  // 0x57c
};

// The playing sounds by preset key: the groups and their keys side by side.
struct UnkSoundGroups_l02423c {
    CRntVector<UnkSoundHandle>* groups;  // 0x68
    u64* keys;                           // 0x70
    u32 count;                           // 0x78

    CRntVector<UnkSoundHandle>* Find(u64 key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &groups[i];
        }
        return &groups[count];
    }
    CRntVector<UnkSoundHandle>* End() { return &groups[count]; }
};

struct UnkSoundManager_l02423c {
    u8 pad_0x0[0x68];
    UnkSoundGroups_l02423c sounds;  // 0x68
};

void fn_710102576c(UnkDropTarget* self, UnkSoundHandle handle);

static inline bool IsPlaying(const UnkSoundHandle& handle) {
    return handle.ref && handle.serial != -1 && handle.ref->serial == handle.serial && handle.ref->object;
}

// The sound's volume; the largest float when the handle no longer plays.
static inline f32 Volume(const UnkSoundHandle& handle) {
    if (IsPlaying(handle))
        return static_cast<UnkPlayingSound_l02423c*>(handle.ref->object)->volume;
    return 3.4028235e38f;
}

bool fn_710102423c(UnkDropTarget* target, UnkAudioPreset_b0038* audio_preset) {
    auto* self = reinterpret_cast<UnkSoundManager_l02423c*>(target);
    auto* preset = reinterpret_cast<UnkPresetLimits_l02423c*>(audio_preset);
    if (!preset)
        return false;
    if (self->sounds.Find(preset->key) == self->sounds.End())
        return true;

    CRntVector<UnkSoundHandle>* group = self->sounds.Find(preset->key);
    if (preset->min_volume >= 0.0f && group->count != 0) {
        f32 quietest = 3.4028235e38f;
        for (UnkSoundHandle* it = group->data; it != group->data + group->count; ++it) {
            f32 volume = Volume(*it);
            quietest = volume < quietest ? volume : quietest;
        }
        if (quietest < preset->min_volume)
            return false;
    }

    s32 limit = preset->max_sounds;
    if (limit < 1 || limit > static_cast<s32>(group->count))
        return true;

    UnkSoundHandle loudest = g_unk_7108b6c460;
    f32 loudest_volume = -3.4028235e38f;
    for (UnkSoundHandle* it = group->data; it != group->data + group->count; ++it) {
        f32 volume = Volume(*it);
        if (volume > loudest_volume) {
            loudest_volume = volume;
            loudest = *it;
        }
    }
    if (!IsPlaying(loudest))
        return false;
    fn_7100105a80(static_cast<UnkSound*>(loudest.ref->object), 0.2f, 0.0f);
    fn_710102576c(target, loudest);
    return true;
}
