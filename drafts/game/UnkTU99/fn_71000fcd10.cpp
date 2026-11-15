// Unit 99 (CTunableSoundSystemATK): the handle of the voice in the first list that has the given name: the first one, or with `lowest` the one with the lowest value at +0x108; the empty handle while
// the queue is off or when none has it. Names and layouts are placeholders.
#include <float.h>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"
#include "game/operator_new.h"
#include "game/sound/UnkSoundHandle.h"

using base::global::CStrId;
using base::snd::UnkSoundSystemATK;

struct UnkVoicePool_b56;
struct UnkSoundVoice2_b56;

struct UnkNamedVoice {
    u8 pad_0x0[0x88];
    base::global::UnkStringData* name;
    u8 pad_0x90[0x78];
    f32 value;
};

// A slot of the voice statistics: what the handle of a voice points at.
struct UnkVoiceStat {
    u64 field_0x0;
    s64 serial;
};

struct UnkNamedSystem {
    u8 pad_0x0[0x398];
    u8 pool[0x28];
    UnkVoiceStat* stats;
    u8 pad_0x3c8[0x3f0 - 0x3c8];
    UnkNamedVoice** list_0x3f0;
    u32 count_0x3f8;
    u8 pad_0x3fc[0x400568 - 0x3fc];
    nn::os::MutexType mutex;
};

UnkSoundHandle fn_71000fcd10(UnkSoundSystemATK* self, const CStrId& id, bool lowest) {
    if (g_unk_7108b6ab60 == 1)
        return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
    UnkNamedSystem* system = reinterpret_cast<UnkNamedSystem*>(self);
    fn_7100132cd0();
    fn_710006691c(&system->mutex);
    UnkNamedVoice* found = nullptr;
    if (system->count_0x3f8 != 0) {
        UnkNamedVoice** voices = system->list_0x3f0;
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        base::global::UnkStringData* wanted = id.data;
        if (lowest) {
            f32 best = FLT_MAX;
            for (u64 i = 0; i < system->count_0x3f8; ++i) {
                UnkNamedVoice* voice = voices[i];
                if (voice->name != empty && voice->name == wanted) {
                    bool lower = best > voice->value;
                    best = lower ? voice->value : best;
                    found = lower ? voice : found;
                }
            }
        } else {
            for (u64 i = 0; i < system->count_0x3f8; ++i) {
                UnkNamedVoice* voice = voices[i];
                if (voice->name != empty && voice->name == wanted) {
                    found = voice;
                    break;
                }
            }
        }
    }
    UnkSoundHandle result;
    if (!found) {
        result = {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
    } else {
        u32 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(system->pool),
                                  reinterpret_cast<UnkSoundVoice2_b56*>(found));
        UnkVoiceStat* stat = system->stats + index;
        result = {reinterpret_cast<UnkSoundRef*>(stat), stat->serial};
    }
    fn_7100066920(&system->mutex);
    return result;
}
