// UnkTU99 (sound system): finds the voice in the system's list whose name matches a file-path id and calls its
// setter with a value. Returns whether one was found. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSound.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkNamedInfo {
    u8 pad_0x0[0x30];
    u64 name;
};

struct UnkNamedVoice {
    u8 pad_0x0[0x90];
    UnkNamedInfo* info;
};

struct UnkNamedOwner {
    u8 pad_0x0[0x418];
    UnkNamedVoice** voices;
    u32 count;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

bool fn_71000fdfd8(UnkNamedOwner* self, const base::global::CFilePathStrId& id, f32 value) {
    bool found;
    if (g_unk_7108b6ab60 & 1) {
        found = false;
    } else {
        found = false;
        fn_7100132cd0();
        fn_710006691c(&self->mutex);
        u64 name;
        {
            base::global::CFilePathStrId key = fn_71000f63b0(id);
            name = reinterpret_cast<const UnkNamedInfo*>(key.data)->name;
        }
        UnkNamedVoice** it = self->voices;
        for (u64 bytes = u64(self->count) * sizeof(*it); bytes != 0; bytes -= sizeof(*it)) {
            UnkNamedVoice* voice = *it++;
            if (voice->info->name == name) {
                fn_7100105a80(reinterpret_cast<UnkSound*>(voice), value, 0.0f);
                found = true;
                break;
            }
        }
        fn_7100066920(&self->mutex);
    }
    return found;
}
