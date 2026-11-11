// Unit 327's constructor (src/game/media/UnkMovieUnit327/UnkMovieUnit327_b50.cpp has the rest of the
// unit): clears the worker thread's state, sets up the multi-wait over the stop event (kind 5) and the
// queue event (kind 4), the mutex and the empty ring, then starts the worker thread
// ("MovieDecoderEventHandler"). Names and layouts are placeholders.
#include <nn/os.h>
#include <stdint.h>

#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU327.h"
#include "types.h"

namespace nn {
namespace os {
struct MultiWaitType;
struct MultiWaitHolderType;
void InitializeMultiWait(MultiWaitType* multiWait);
void InitializeMultiWaitHolder(MultiWaitHolderType* holder, EventType* event);
void SetMultiWaitHolderUserData(MultiWaitHolderType* holder, u64 user_data);
void LinkMultiWaitHolder(MultiWaitType* multiWait, MultiWaitHolderType* holder);
}  // namespace os
}  // namespace nn

void fn_7100066744(void* thread, const char* name, void (*entry)(void*), u32 a, u32 b, void* arg);

struct UnkMovieWorker_b50;
void fn_7100222ab4(UnkMovieWorker_b50* thread);

namespace {

struct EventInfo_ovb0259 {
    u32 kind;
    nn::os::EventType* event;
    void* field_0x10;
};

struct WaitedEvent_ovb0259 {
    nn::os::EventType event;
    u8 holder[0x40];
    bool linked;
    EventInfo_ovb0259 info;
};

struct Entry_ovb0259 {
    void* field_0x0;
    bool field_0x8;
};

}  // namespace

struct UnkMovieUnit327_b50;

struct UnkMovieUnit327_ovb0259 {
    u8 pad_0x0[0x1e4];
    u32 field_0x1e4[5];  // the worker thread's state
    u8 pad_0x1f8[0x30];  // the multi-wait
    WaitedEvent_ovb0259 field_0x228;
    WaitedEvent_ovb0259 field_0x2b0;
    nn::os::MutexType field_0x338;
    void** field_0x358;
    Entry_ovb0259* field_0x360;
    Entry_ovb0259* field_0x368;
    u64 field_0x370;
    Entry_ovb0259 field_0x378[64];
    s32 field_0x778;
};
static_assert(__builtin_offsetof(UnkMovieUnit327_ovb0259, field_0x378) == 0x378, "layout");

static inline void InitWaited(UnkMovieUnit327_ovb0259* self, WaitedEvent_ovb0259* w, u32 kind) {
    w->info.kind = kind;
    w->info.event = &w->event;
    w->info.field_0x10 = nullptr;
    nn::os::InitializeEvent(&w->event, false, nn::os::EventClearMode_ManualClear);
    nn::os::MultiWaitHolderType* holder = reinterpret_cast<nn::os::MultiWaitHolderType*>(w->holder);
    nn::os::InitializeMultiWaitHolder(holder, &w->event);
    nn::os::SetMultiWaitHolderUserData(holder, reinterpret_cast<u64>(&w->info));
    nn::os::LinkMultiWaitHolder(reinterpret_cast<nn::os::MultiWaitType*>(self->pad_0x1f8), holder);
    w->linked = true;
}

void fn_7100222970(UnkMovieUnit327_b50* self_) {
    UnkMovieUnit327_ovb0259* self = reinterpret_cast<UnkMovieUnit327_ovb0259*>(self_);
    for (int i = 0; i < 5; ++i)
        self->field_0x1e4[i] = 0;
    self->field_0x358 = g_unk_7101a20570 + 2;
    self->field_0x228.linked = false;
    self->field_0x2b0.linked = false;
    nn::os::MutexType* mutex = &self->field_0x338;
    Entry_ovb0259* ring = self->field_0x378;
    self->field_0x370 = 0;
    self->field_0x778 = 0;
    self->field_0x360 = ring;
    self->field_0x368 = ring;
    fn_71000668f8(mutex);
    nn::os::InitializeMultiWait(reinterpret_cast<nn::os::MultiWaitType*>(self->pad_0x1f8));
    InitWaited(self, &self->field_0x228, 5);
    InitWaited(self, &self->field_0x2b0, 4);
    fn_7100066744(self, "MovieDecoderEventHandler", reinterpret_cast<void (*)(void*)>(fn_7100222ab4), 0, 0x10, self);
}
