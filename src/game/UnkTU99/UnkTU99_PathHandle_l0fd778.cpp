// Unit 99 (CTunableSoundSystemATK): the handle of the first voice in the second list whose file path is the
// one named (sanitised by fn_71000f63b0); the empty handle while the queue is off, an invalid one when no
// voice has the path. Names and layouts are placeholders.
#include <new>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"
#include "game/operator_new.h"
#include "game/sound/UnkSoundHandle.h"

using base::global::CFilePathStrId;
using base::snd::UnkSoundSystemATK;

struct UnkVoicePool_b56;
struct UnkSoundVoice2_b56;

struct UnkVoicePath {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkPathVoice {
    u8 pad_0x0[0x90];
    UnkVoicePath* path;
};

// A slot of the voice statistics: what the handle of a voice points at.
struct UnkVoiceStat {
    u64 field_0x0;
    s64 serial;
};

struct UnkPathSystem {
    u8 pad_0x0[0x398];
    u8 pool[0x28];
    UnkVoiceStat* stats;
    u8 pad_0x3c8[0x418 - 0x3c8];
    UnkPathVoice** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

// Storage for a file-path id whose release is written out by hand.
union UnkPathIdSlot {
    CFilePathStrId id;
    UnkPathIdSlot() {}
    ~UnkPathIdSlot() {}
};

UnkSoundHandle fn_71000fd778(UnkSoundSystemATK* self, const CFilePathStrId& path) {
    if (g_unk_7108b6ab60 == 1)
        return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
    UnkPathSystem* system = reinterpret_cast<UnkPathSystem*>(self);
    fn_7100132cd0();
    fn_710006691c(&system->mutex);
    UnkPathIdSlot slot;
    new (&slot.id) CFilePathStrId(fn_71000f63b0(path));
    base::global::UnkStringData* data = slot.id.data;
    u64 key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(data) + 0x30);
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
        if (data && data != &pool->empty) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47200;
            }
        }
        slot.id.data = &pool->empty;
    }
    UnkSoundHandle result = {nullptr, -1};
    for (UnkPathVoice **it = system->list_0x418, **end = it + system->count_0x420; it != end; ++it) {
        if ((*it)->path->key == key) {
            u32 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(system->pool),
                                      reinterpret_cast<UnkSoundVoice2_b56*>(*it));
            UnkVoiceStat* stat = system->stats + index;
            result = {reinterpret_cast<UnkSoundRef*>(stat), stat->serial};
            break;
        }
    }
    fn_7100066920(&system->mutex);
    return result;
}
