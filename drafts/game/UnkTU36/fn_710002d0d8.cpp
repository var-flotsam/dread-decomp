// Unit 36: a worker thread's main loop. Each round runs the queued jobs through a virtual handler,
// records how far it got, signals the owner and sleeps out the rest of a 600 ms period.
// Names and layouts are placeholders.

#include <nn/os.h>
#include <nn/os/os_Event.h>
#include <nn/os/os_TickTypes.h>

#include "types.h"

struct UnkJob_0x40 {
    u8 pad_0x0[0x40];
};

class UnkJobWorker {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18(u32 id, UnkJob_0x40* job);

    bool running;
    u8 field_0x9;
    u8 pad_0xa[2];
    s32 jobCount;
    u32 jobIds[16];
    UnkJob_0x40 jobs[16];
    s32 jobsDone;
    u32 lastJobId;
    s32 lastResult;
    u8 pad_0x45c[4];
    nn::os::EventType* doneEvent;
    u8 pad_0x468[0x628 - 0x468];
    nn::os::EventType startEvent;
};
static_assert(__builtin_offsetof(UnkJobWorker, jobsDone) == 0x450, "");
static_assert(__builtin_offsetof(UnkJobWorker, startEvent) == 0x628, "");

extern u8 g_unk_7101d55004;

// x / 1000000 written out as the high half of a 64x64-bit product built from 32-bit halves, which is
// how the original computes it (a plain division compiles to smulh).
static inline s64 DivideByMillion(s64 x) {
    const u64 lo = static_cast<u32>(x);
    const s64 hi = x >> 32;
    const u64 mlo = 0xd7b634db;
    const u64 mhi = 0x431bde82;
    const s64 t = hi * static_cast<s64>(mlo) + static_cast<s64>((lo * mlo) >> 32);
    const s64 u = hi * static_cast<s64>(mhi);
    const u64 v = lo * mhi + static_cast<u32>(t);
    const s64 h = u + (t >> 32) + static_cast<s64>(v >> 32);
    return (h >> 18) + static_cast<s64>(static_cast<u64>(x) >> 63);
}

void fn_710002d0d8(UnkJobWorker* self) {
    nn::os::WaitEvent(&self->startEvent);
    while (self->running) {
        nn::os::Tick start = nn::os::GetSystemTick();

        s32 i = 0;
        s32 result = 0;
        u32 id = 0;
        for (; i < self->jobCount; i++) {
            if (result != 0 || g_unk_7101d55004) {
                break;
            }
            id = self->jobIds[i];
            result = self->vf_0x18(id, &self->jobs[i]);
        }
        self->lastResult = result;
        self->lastJobId = id;
        self->jobsDone = i;
        self->jobCount = 0;
        self->field_0x9 = 0;

        nn::os::Tick elapsed = nn::os::GetSystemTick() - start;
        s64 ms = DivideByMillion(nn::os::ConvertToTimeSpan(elapsed).nanoseconds);
        if (ms < 600) {
            nn::os::SleepThread(nn::TimeSpan::FromMilliSeconds(600 - ms));
        }
        nn::os::SignalEvent(self->doneEvent);
        nn::os::WaitEvent(&self->startEvent);
    }
}
