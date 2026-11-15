// Unit 99 (CTunableSoundSystemATK): voice command 1: turns on a voice's flag (found by id in one of the two
// voice lists) when the payload asks for it and it is not on yet. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"

using base::snd::UnkSoundSystemATK;

struct UnkFlagVoice {
    u8 pad_0x0[0xc4];
    s32 id;
    u8 pad_0xc8[0x59];
    u8 enabled;
};

struct UnkFlagSystem {
    u8 pad_0x0[0x3f0];
    UnkFlagVoice** list_0x3f0;
    u32 count_0x3f8;
    u8 pad_0x3fc[0x1c];
    UnkFlagVoice** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

// Runs `set` on the first voice of the list with id `id`.
template <class F>
static inline void ForVoice(UnkFlagVoice** voices, u32 count, s32 id, F set) {
    for (UnkFlagVoice **it = voices, **end = voices + count; it != end; ++it) {
        if ((*it)->id == id) {
            set(*it);
            break;
        }
    }
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100100088(UnkSoundSystemATK* self, const s32* payload) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    UnkFlagSystem* system = reinterpret_cast<UnkFlagSystem*>(self);
    fn_7100132cd0();
    fn_710006691c(&system->mutex);
    auto set = [payload](UnkFlagVoice* voice) {
        if (voice->enabled != (payload[2] != 0) && payload[2] != 0)
            fn_7100105034(voice);
    };
    if (payload[1] != 0)
        ForVoice(system->list_0x418, system->count_0x420, payload[0], set);
    else
        ForVoice(system->list_0x3f0, system->count_0x3f8, payload[0], set);
    fn_7100066920(&system->mutex);
}
