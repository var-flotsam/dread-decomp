// Unit 36 of tools/tu_map.py (0x710002a028-0x710002d964, no reflected class; its static initialiser
// is src/game_s1/fn_710002d840.cpp): an object served by a worker thread. Holds fn_710002d3e0, which
// posts a request: marks it pending (+0x9), clears the result (+0x450), keeps the caller's event
// (+0x460, cleared for the answer) and wakes the worker (the event at +0x628). Names and layouts
// are placeholders.
#include <nn/os.h>

#include "types.h"

struct UnkUnit36Object {
    u8 pad_0x0[0x9];
    bool pending;
    u8 pad_0xa[0x446];
    u32 result;  // 0x450
    u8 pad_0x454[0xc];
    nn::os::EventType* done;  // 0x460
    u8 pad_0x468[0x1c0];
    // The worker's event (+0x628), as raw storage: the port's runtime declares nn::os::EventType
    // without defining it.
    u8 wake[0x28];
};
static_assert(__builtin_offsetof(UnkUnit36Object, wake) == 0x628, "UnkUnit36Object layout");

// NON_MATCHING(minor): the original stores the flag before the result (scheduling); same behaviour.
void fn_710002d3e0(UnkUnit36Object* self, nn::os::EventType* done) {
    self->pending = true;
    self->result = 0;
    self->done = done;
    nn::os::ClearEvent(done);
    nn::os::SignalEvent(reinterpret_cast<nn::os::EventType*>(self->wake));
}
