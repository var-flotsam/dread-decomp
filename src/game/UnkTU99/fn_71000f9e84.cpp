// Unit 99 (CTunableSoundSystemATK): runs the queued sound commands of the finished buffer, then swaps the
// buffers under the mutex. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU99.h"

using base::snd::UnkSoundSystemATK;


void fn_71000ffe2c(UnkSoundSystemATK* self, u32 command, const s32* payload);

// The members of the sound system the buffer swap touches.
struct UnkSoundQueue {
    u8 pad_0x0[0x510];
    s32 current;
    s32 other;
    s32 used;
    s32 pending;
    u8 field_0x520[2][0x100000];
    u8 field_0x200520[2][0x100000];
    u8 pad_0x400520[0x28];
    nn::os::MutexType mutex;
};
static_assert(__builtin_offsetof(UnkSoundQueue, mutex) == 0x400548, "UnkSoundQueue placeholder layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71000f9e84(UnkSoundSystemATK* self) {
    UnkSoundQueue* queue = reinterpret_cast<UnkSoundQueue*>(self);
    fn_7100132cd0();
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    if (queue->used > 0) {
        u8* buffer = queue->field_0x200520[queue->current];
        s32 offset = 0;
        do {
            s32 command = *reinterpret_cast<s32*>(buffer + offset);
            offset += 4;
            const s32* payload = reinterpret_cast<const s32*>(buffer + offset);
            offset += g_unk_7101647aa8[command];
            fn_71000ffe2c(self, command, payload);
        } while (offset < queue->used);
    }
    fn_710006691c(&queue->mutex);
    queue->current = queue->current == 0;
    queue->other = queue->other == 0;
    queue->used = queue->pending;
    queue->pending = 0;
    fn_7100066920(&queue->mutex);
}
