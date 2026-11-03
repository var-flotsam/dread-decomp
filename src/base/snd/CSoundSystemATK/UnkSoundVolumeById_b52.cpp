// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK): looks a sound up by id in the system's map at
// +0x400788 (the id map layout of src/base/snd/CSoundSystemATK.cpp) and, when it is there and not flagged
// (+0x79), sets its volume from a level: a fixed value below 80 and at 16000 or more, else a logarithmic
// one, clamped to [-1, 0] (fn_71000f424c), then fn_71000f42c0(-1, 0). Names are placeholders.
#include <math.h>

#include "types.h"

struct UnkSoundById_b52 {
    u8 pad_0x0[0x79];
    bool field_0x79;
};
void fn_71000f424c(UnkSoundById_b52* sound, f32 volume);
void fn_71000f42c0(UnkSoundById_b52* sound, s32 a, f32 b);

struct UnkSoundIdNode_b52 {
    u32 key;
    UnkSoundById_b52* sound;
    u8 pad_0x10[0x10];
    UnkSoundIdNode_b52* next;
};

struct UnkSoundIdMap_b52 {
    u8 pad_0x0[0x10];
    UnkSoundIdNode_b52** buckets;
    u32 used;
    u32 bucket_count;
};

struct UnkSoundSystemIds_b52 {
    u8 pad_0x0[0x400788];
    UnkSoundIdMap_b52 sounds;
};

struct UnkSoundLevel_b52 {
    u32 id;
    u32 level;
};

void fn_7100102918(UnkSoundSystemIds_b52* self, const UnkSoundLevel_b52* params) {
    UnkSoundIdMap_b52& map = self->sounds;
    if (!map.used)
        return;
    u32 key = params->id;
    u32 index = key % map.bucket_count;
    UnkSoundIdNode_b52** bucket = &map.buckets[index];
    if (!bucket)
        return;
    UnkSoundIdNode_b52* node = *bucket;
    for (; node; node = node->next) {
        if (node->key == key)
            break;
    }
    if (!node)
        return;
    UnkSoundById_b52* sound = node->sound;
    if (!sound || sound->field_0x79)
        return;
    u32 level = params->level;
    f32 volume;
    if (level < 80)
        volume = -0x1.ba90c0p-1f;
    else if (level >= 16000)
        volume = -0x1.9999a0p-4f;
    else {
        volume = logf((f32)level) * 0x1.2776c6p-3f + -0x1.fc7f0ep-2f + -1.0f;
        sound = node->sound;
    }
    f32 clamped = volume <= -1.0f ? -1.0f : (volume >= 0.0f ? 0.0f : volume);
    fn_71000f424c(sound, clamped);
    fn_71000f42c0(node->sound, -1, 0.0f);
}
