// The engine's thread wrapper (the unit before base::global::timeline::CEvent): an nn::os thread,
// two light events, a state word and the stack it allocated. fn_7100066744 creates and starts it
// (the movie player's "MovieAudioStreamNx Thread", src/game_71001e2acc/fn_7100221534.cpp, and
// the graphics system's "LoadingScreenThread", src/engine/UnkGraphicsSystem.cpp); the states are
// 1 created, 2 released, 3 stopping and 4 destroyed.

#include <atomic>

#include <nn/os.h>

#include "base/UnkVector.h"
#include "types.h"

struct UnkEngineThread {
    nn::os::ThreadType thread;          // +0x0
    nn::os::LightEventType startEvent;  // +0x1c0: what the thread waits on before running
    nn::os::LightEventType event_0x1cc; // +0x1cc
    u8 unk_0x1d8[0xc];
    std::atomic<s32> state;             // +0x1e4
    void* stack;                        // +0x1e8: from the default allocator
};
static_assert(sizeof(nn::os::ThreadType) == 0x1c0, "ThreadType size");

// Lets a created thread run: always when bit 0 of `force` is set, else only from state 1. Sets
// state 2.
void fn_7100066828(void* thread, u32 force) {
    UnkEngineThread* self = static_cast<UnkEngineThread*>(thread);
    if (!(force & 1) && self->state != 1)
        return;
    self->state = 2;
    nn::os::SignalLightEvent(&self->startEvent);
}

// Destroys a created or released thread: releases it (so it can return), destroys it, frees its
// stack and finalizes both events. Other states are left alone.
void fn_710006686c(void* thread) {
    UnkEngineThread* self = static_cast<UnkEngineThread*>(thread);
    if (self->state == 1 || self->state == 2) {
        self->state = 3;
        nn::os::SignalLightEvent(&self->startEvent);
        nn::os::DestroyThread(&self->thread);
        fn_7100083990()->vf_0x48(self->stack);
        self->stack = nullptr;
        nn::os::FinalizeLightEvent(&self->event_0x1cc);
        nn::os::FinalizeLightEvent(&self->startEvent);
        self->state = 4;
    }
}
