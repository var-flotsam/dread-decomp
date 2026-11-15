// Unit 99 (CTunableSoundSystemATK): applies a second fade (value) in or out to the first voice in the second list
// whose file path is the one named (sanitised by fn_71000f63b0); returns whether one was found. Names and
// layouts are placeholders.
#include <new>

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

// Storage for a file-path id whose release is written out by hand.
union UnkPathIdSlot {
    CFilePathStrId id;
    UnkPathIdSlot() {}
    ~UnkPathIdSlot() {}
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

bool fn_71000fe774(UnkSoundSystemATK* self, const CFilePathStrId& path, f32 value, bool on) {
    bool found;
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1)) {
        found = false;
    } else {
        UnkPathSystem* system = reinterpret_cast<UnkPathSystem*>(self);
        fn_7100132cd0();
        fn_710006691c(&system->mutex);
        UnkPathIdSlot slot;
        new (&slot.id) CFilePathStrId(fn_71000f63b0(path));
        base::global::UnkStringData* data = slot.id.data;
        u64 key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(data) + 0x30);
        // The sanitised id is released by hand, as the original does.
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            slot.id.data = &pool->empty;
        }
        found = false;
        for (UnkPathVoice **it = system->list_0x418, **end = it + system->count_0x420; it != end; ++it) {
            if ((*it)->path->key == key) {
                if (on)
                    fn_7100105c9c(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
                else
                    fn_7100105cf0(reinterpret_cast<UnkPlayingSound_ovb0074*>(*it), value);
                found = true;
                break;
            }
        }
        fn_7100066920(&system->mutex);
    }
    return found;
}
