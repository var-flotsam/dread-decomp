// UnkTU99 (sound system): under the lock, finds the playing sound whose file-path id matches a name and updates it.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkNameData_e56c {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkPlaying_e56c {
    u8 pad_0x0[0x90];
    UnkNameData_e56c* name;
};

struct UnkOwner_e56c {
    u8 pad_0x0[0x418];
    UnkPlaying_e56c** sounds;
    u32 count;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

// Reads the id's key, then releases the id's reference as the id's destructor would.
static inline u64 LookupKey(const base::global::CFilePathStrId& id) {
    union Holder {
        base::global::CFilePathStrId id;
        Holder(const base::global::CFilePathStrId& other) : id(fn_71000f63b0(other)) {}
        ~Holder() {}
    } found(id);
    base::global::UnkStringData* data = found.id.data;
    u64 key = reinterpret_cast<UnkNameData_e56c*>(data)->key;
    base::global::UnkStringPool** slot = &g_unk_7101d47200;
    // keeps the pool slot's address in a register across the release call, as the original does
    asm("" : "+r"(slot));
    if (base::global::UnkStringPool* pool = *slot) {
        if (data && &pool->empty != data) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = *slot;
            }
        }
        found.id.data = &pool->empty;
    }
    return key;
}

void fn_71000fe56c(UnkOwner_e56c* self, const base::global::CFilePathStrId& id, f32 a, f32 b) {
    if (__builtin_expect(!(g_unk_7108b6ab60 & 1), 0)) {
        fn_7100132cd0();
        fn_710006691c(&self->mutex);
        u64 key = LookupKey(id);
        for (u32 i = 0; i < self->count; i++) {
            UnkPlaying_e56c* sound = self->sounds[i];
            if (sound->name->key == key) {
                fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), b, false, a);
                break;
            }
        }
        fn_7100066920(&self->mutex);
    }
}
