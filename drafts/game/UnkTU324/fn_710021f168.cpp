// Unit 324 (media instance): sets the state at +0x2c under the listener lock, keeping the old one at +0x30,
// and tells the listeners unless the new state is 1. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU324.h"
#include "types.h"

struct UnkMedia324;

struct UnkMediaListener324 {
    void* context;
    void (*callback)(void* context, UnkMedia324* media, u32 arg);
};

struct UnkMedia324 {
    u8 pad_0x0[0x2c];
    s32 state;           // 0x2c
    s32 previous_state;  // 0x30
    u8 pad_0x34[0x6ec];
    UnkMediaListener324* listeners;  // 0x720
    u32 listener_count;              // 0x728
    u8 pad_0x72c[0x214];
    nn::os::MutexType listener_lock;  // 0x940
};

bool fn_710021f168(UnkMedia324* self, s32 state) {
    fn_710006691c(&self->listener_lock);
    bool notified = false;
    if (state >= 0) {
        self->previous_state = self->state;
        self->state = state;
        if (state != 1) {
            for (UnkMediaListener324* it = self->listeners, *end = it + self->listener_count; it != end; ++it)
                it->callback(it->context, self, 0);
            notified = true;
        }
    }
    fn_7100066920(&self->listener_lock);
    return notified;
}
