// Unit 99 (CTunableSoundSystemATK): the value of the first voice in the second list whose file path is the
// one named (sanitised by fn_71000f63b0), under the voice mutex; 0 when none or while the global flag is
// set. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
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
    u8 pad_0x98[0x54];
    f32 value;
};

struct UnkPathSystem {
    u8 pad_0x0[0x418];
    UnkPathVoice** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

f32 fn_71000fe680(UnkSoundSystemATK* self, const CFilePathStrId& path) {
    f32 value;
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1)) {
        value = 0.0f;
    } else {
        UnkPathSystem* system = reinterpret_cast<UnkPathSystem*>(self);
        fn_7100132cd0();
        fn_710006691c(&system->mutex);
        u64 key;
        {
            CFilePathStrId sanitized = fn_71000f63b0(path);
            key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(sanitized.data) + 0x30);
        }
        value = 0.0f;
        for (UnkPathVoice **it = system->list_0x418, **end = it + system->count_0x420; it != end; ++it) {
            if ((*it)->path->key == key) {
                value = (*it)->value;
                break;
            }
        }
        fn_7100066920(&system->mutex);
    }
    return value;
}
