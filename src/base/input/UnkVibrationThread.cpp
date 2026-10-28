// From unit 47 of tools/tu_map.py (0x710006f390-0x7100070740, no class hint): the vibration thread's
// loop, which runs the nn::hid vibration nodes' sampling once per sample interval until the thread
// is told to stop. Names and layouts are placeholders.
// Holds fn_710006f390.
#include "base/input/UnkHidVibration.h"
#include "types.h"

// The SDK's sample interval (nn::hid::VibrationNode::DefaultVibrationSampleInterval, an nn::TimeSpan
// in nanoseconds), named by its symbol: the stand-in class in base/input/UnkHidVibration.h has no
// static data.
extern const s64 g_DefaultVibrationSampleInterval asm("_ZN2nn3hid13VibrationNode30DefaultVibrationSampleIntervalE");

// The engine's clock: milliseconds since its start stamp (src/game_s1/fn_7100080c40.cpp).
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;
struct UnkTimer_7100080c40;
s64 fn_7100080f2c(const UnkTimer_7100080c40* self);

// Sleeps the calling thread for `ms` milliseconds (the first argument is not read).
void fn_7100066854(void*, s32 ms);

struct UnkVibrationThread {
    u8 pad_0x0[0x1e4];
    s32 state;  // 3: stop (read with acquire ordering)
};

static inline s64 Now() { return fn_7100080f2c(reinterpret_cast<const UnkTimer_7100080c40*>(g_unk_7101d47328)); }

// The interval in milliseconds, computed once.
static inline s64 IntervalMs() {
    static const s64 s_ms = g_DefaultVibrationSampleInterval / 1000000;
    return s_ms;
}

// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_710006f390(UnkVibrationThread* self) {
    s64 last = Now();
    while (__atomic_load_n(&self->state, __ATOMIC_ACQUIRE) != 3) {
        s64 start = Now();
        if (start != last)
            nn::hid::VibrationNode::Update();
        s64 end = Now();
        u64 spent = (u64)(end - start);
        if (spent < (u64)IntervalMs())
            fn_7100066854(self, (s32)IntervalMs() - (s32)spent);
        last = start;
    }
}
