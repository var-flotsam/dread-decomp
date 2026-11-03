#include "types.h"

struct UnkSyncBlockOwner {
    u8 unk_0x0[0x10];
    void* target;
};

struct CSynchronizedBlockCameraCtrl {
    u8 unk_0x0[0xb0];
    UnkSyncBlockOwner* owner;
    u8 unk_0xb8[0x2e8 - 0xb8];
    bool finished;  // 0x2e8
    u8 unk_0x2e9[0x304 - 0x2e9];
    f32 remaining;  // 0x304
};

void fn_7100763f9c(CSynchronizedBlockCameraCtrl* self, f32 dt);

// The base camera update, then counts the remaining time down while there is a target; marks the
// control finished when it runs out.
void fn_7100a21770(CSynchronizedBlockCameraCtrl* self, f32 dt) {
    fn_7100763f9c(self, dt);
    if (self->owner && self->owner->target && self->remaining > 0.0f) {
        f32 left = self->remaining - dt;
        left = left < 0.0f ? 0.0f : left;
        self->remaining = left;
        if (left <= 0.0f)
            self->finished = true;
    }
}
