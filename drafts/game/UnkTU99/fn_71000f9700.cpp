// Unit 99 (CTunableSoundSystemATK): under the system's mutex, after running the finished command buffer,
// stops the voices of both lists that are idle (or all of them, by flag), and with `release` also sends
// command 5 for every pending id and clears its map entry. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU99.h"

using base::global::CRntVector;
using base::snd::UnkSoundSystemATK;

struct UnkSoundSystemVoices_b56;
struct UnkSoundVoice2_b56;
bool fn_71000fbf2c(UnkSoundSystemVoices_b56* self, UnkSoundVoice2_b56* voice, bool flag);
bool fn_71000fc074(UnkSoundSystemVoices_b56* self, UnkSoundVoice2_b56* voice);
void fn_71000f9e84(UnkSoundSystemATK* self);

struct UnkIdleVoice_l0f9700 {
    u8 pad_0x0[0x110];
    f32 level;  // 0x110
    u8 pad_0x114[5];
    bool playing;  // 0x119
};

// Whether a voice plays and its level has fallen to zero or below.
static inline bool IsIdle(const UnkIdleVoice_l0f9700* voice) { return voice->playing && !(voice->level > 0.0f); }

struct UnkIdleNode_l0f9700 {
    u64 key;
    u32 value;
    u8 pad_0xc[0x14];
    UnkIdleNode_l0f9700* chain;  // 0x20
};

struct UnkIdleSystem_l0f9700 {
    u8 pad_0x0[0x3f0];
    CRntVector<UnkIdleVoice_l0f9700*> flagged;  // 0x3f0
    CRntVector<UnkIdleVoice_l0f9700*> plain;    // 0x418
    u8 pad_0x440[0x468 - 0x440];
    CRntVector<u64> pending;  // 0x468
    u8 pad_0x490[0x4c8 - 0x490];
    UnkIdleNode_l0f9700** buckets;  // 0x4c8
    u32 used;                       // 0x4d0
    u32 bucket_count;               // 0x4d4
    u8 pad_0x4d8[0x400568 - 0x4d8];
    nn::os::MutexType mutex;  // 0x400568

    UnkIdleNode_l0f9700* Find(u64 key) {
        if (!used)
            return nullptr;
        UnkIdleNode_l0f9700** bucket = static_cast<u32>(key) % bucket_count + buckets;
        if (!bucket)
            return nullptr;
        for (UnkIdleNode_l0f9700* n = *bucket; n; n = n->chain) {
            if (n->key == key)
                return n;
        }
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkIdleSystem_l0f9700, pending) == 0x468, "layout");
static_assert(__builtin_offsetof(UnkIdleSystem_l0f9700, mutex) == 0x400568, "layout");

// The voices picked to stop; its destructor is fn_71000f76d4.
struct UnkIdleBatch_l0f9700 : CRntVector<UnkIdleVoice_l0f9700*> {
    UnkIdleBatch_l0f9700() : CRntVector(0xf) {}
    ~UnkIdleBatch_l0f9700();
};

// Command 5's payload.
struct UnkReleasePayload_l0f9700 {
    u64 id;
    u32 flag;
};

void fn_71000f9700(void* member, bool all_flagged, bool release, bool all_plain) {
    if ((g_unk_7108b6ab60 & 1) == 0) {
        UnkIdleSystem_l0f9700* self = static_cast<UnkIdleSystem_l0f9700*>(member);
        UnkSoundSystemVoices_b56* voices = static_cast<UnkSoundSystemVoices_b56*>(member);
        fn_7100132cd0();
        nn::os::MutexType* mutex = &self->mutex;
        fn_710006691c(mutex);
        fn_71000f9e84(static_cast<UnkSoundSystemATK*>(member));

        static UnkIdleBatch_l0f9700 s_batch;
        s_batch.Clear();
        for (UnkIdleVoice_l0f9700 **it = self->flagged.data, **end = it + self->flagged.count; it != end; ++it) {
            if (all_flagged || IsIdle(*it))
                s_batch.PushBack(*it);
        }
        for (UnkIdleVoice_l0f9700 **it = s_batch.data, **end = it + s_batch.count; it != end; ++it)
            fn_71000fbf2c(voices, reinterpret_cast<UnkSoundVoice2_b56*>(*it), release);

        s_batch.Clear();
        for (UnkIdleVoice_l0f9700 **it = self->plain.data, **end = it + self->plain.count; it != end; ++it) {
            if (all_plain || IsIdle(*it))
                s_batch.PushBack(*it);
        }
        for (UnkIdleVoice_l0f9700 **it = s_batch.data, **end = it + s_batch.count; it != end; ++it)
            fn_71000fc074(voices, reinterpret_cast<UnkSoundVoice2_b56*>(*it));

        if (release) {
            CRntVector<u64>& pending = self->pending;
            for (u64* it = pending.data; it != pending.data + pending.count;) {
                u64 id = *it;
                UnkReleasePayload_l0f9700 payload;
                payload.id = id;
                payload.flag = 1;
                fn_71000faac4(static_cast<UnkSoundSystemATK*>(member), 5, &payload);
                u32 index = static_cast<u32>(it - pending.data);
                pending.EraseAt(index);
                it = pending.data + index;
                if (UnkIdleNode_l0f9700* node = self->Find(id))
                    node->value = 0;
            }
            pending.Clear();
        }
        fn_7100066920(mutex);
    }
}
