// Unit 329 (tools/tu_map.py: 0x71002236ac-0x7100224d44): the media player singleton behind
// g_unk_716e554378 (UnkPlayer, 0x1278 bytes; created in src/game/UnkRenderManager.cpp). Names and
// layouts are placeholders.
#include <nn/os.h>

#include "types.h"

// The player as its worker sees it: +0x20 is nonzero while it runs, +0x11e0 the queue of requests.
struct UnkPlayerWork_b50 {
    u8 pad_0x0[0x20];
    s32 field_0x20;
    u8 pad_0x24[0x11e0 - 0x24];
    nn::os::MessageQueueType field_0x11e0;
};

// The worker's thread: +0x1e4 is its state (atomic; 3 = stopping), +0x1f0 its argument.
struct UnkPlayerThread_b50 {
    u8 pad_0x0[0x1e4];
    s32 field_0x1e4;
    u8 pad_0x1e8[0x8];
    UnkPlayerWork_b50* field_0x1f0;
};

// A queued request: what to run is at +0x30.
struct UnkPlayerRequest_b50 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

void fn_7100225314(void* item);

// The worker: while the player runs, takes requests (waiting up to 10 ms each) and runs them.
void fn_7100223b8c(UnkPlayerThread_b50* thread) {
    UnkPlayerWork_b50* self = thread->field_0x1f0;
    while (__atomic_load_n(&thread->field_0x1e4, __ATOMIC_ACQUIRE) != 3 && self->field_0x20 != 0) {
        u64 message = 0;
        if (nn::os::TimedReceiveMessageQueue(&message, &self->field_0x11e0, nn::TimeSpan::FromMilliSeconds(10)) &&
            message != 0)
            fn_7100225314(reinterpret_cast<UnkPlayerRequest_b50*>(message)->field_0x30);
    }
}
