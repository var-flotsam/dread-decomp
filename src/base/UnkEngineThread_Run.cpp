// The body of the engine's thread wrapper (src/base/UnkEngineThread.cpp; fn_7100066744 passes it to
// nn::os::CreateThread): waits to be released, then runs the entry at +0x1d8 on the wrapper, signals
// the second event and goes back to waiting (state 1), until the state is 3 (stopping). Names and
// layouts are placeholders.
#include <atomic>

#include <nn/os.h>

#include "types.h"

struct UnkEngineThreadRun {
    nn::os::ThreadType thread;          // +0x0
    nn::os::LightEventType startEvent;  // +0x1c0
    nn::os::LightEventType doneEvent;   // +0x1cc
    void (*entry)(UnkEngineThreadRun*); // +0x1d8
    u32 unk_0x1e0;
    std::atomic<s32> state;             // +0x1e4
};

void fn_71000666b0(void* arg) {
    UnkEngineThreadRun* self = static_cast<UnkEngineThreadRun*>(arg);
    nn::os::WaitLightEvent(&self->startEvent);
    while (self->state != 3) {
        nn::os::ClearLightEvent(&self->doneEvent);
        self->entry(self);
        nn::os::SignalLightEvent(&self->doneEvent);
        if (self->state == 3)
            break;
        self->state = 1;
        nn::os::WaitLightEvent(&self->startEvent);
    }
}
