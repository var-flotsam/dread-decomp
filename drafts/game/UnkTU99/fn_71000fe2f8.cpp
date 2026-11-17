// Unit 99 (CTunableSoundSystemATK): applies a fade (value) in or out to the first voice in the second list
// whose file path is the one named (sanitised by fn_71000f63b0); returns whether one was found. Names and
// layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"

using base::global::CFilePathStrId;
using base::snd::UnkSoundSystemATK;

struct UnkVoicePath {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkPathVoice {
    u8 pad_0x0[0x90];
    UnkVoicePath* path;
};

struct UnkPathSystem {
    u8 pad_0x0[0x418];
    UnkPathVoice** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

bool fn_71000fe2f8(UnkSoundSystemATK* self, const CFilePathStrId& path, f32 value, bool on) {
    bool found = false;
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1)) {
        found = false;
    } else {
        UnkPathSystem* system = reinterpret_cast<UnkPathSystem*>(self);
        fn_7100132cd0();
        fn_710006691c(&system->mutex);
        u64 key;
        {
            CFilePathStrId sanitized = fn_71000f63b0(path);
            key = sanitized.data->Key();
        }
        for (UnkPathVoice **it = system->list_0x418, **end = it + system->count_0x420; it != end; ++it) {
            if ((*it)->path->key == key) {
                if (on)
                    fn_7100105bac(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
                else
                    fn_7100105c00(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
                found = true;
                break;
            }
        }
        fn_7100066920(&system->mutex);
    }
    return found;
}
