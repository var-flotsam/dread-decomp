// nn::atk (tools/tu_map.py, library unit): while the thread state (+0x8, re-read each time) is 2, runs
// fn_7101238990 on the member at 0x2d0, then, if the state is still set, posts 1 to the message queue at
// 0x218. Names and layout are placeholders.
#include <nn/os.h>

#include "types.h"

void fn_7101238990(void* self);

struct UnkAtkThread_b48a3 {
    u8 pad_0x0[0x8];
    volatile s32 state;
    u8 pad_0xc[0x218 - 0xc];
    u8 queue[0x2d0 - 0x218];  // nn::os::MessageQueueType
    u8 field_0x2d0[1];
};

void fn_7101238abc(UnkAtkThread_b48a3* self) {
    if (self->state == 0 || self->state != 2)
        return;
    fn_7101238990(self->field_0x2d0);
    if (self->state == 0)
        return;
    nn::os::TrySendMessageQueue(reinterpret_cast<nn::os::MessageQueueType*>(self->queue), 1);
}
