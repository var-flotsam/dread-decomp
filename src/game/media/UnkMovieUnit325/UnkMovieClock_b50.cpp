// Unit 325 (tools/tu_map.py: 0x71002206fc-0x7100220adc), next to the media::CMovie* units: a playback
// clock under a mutex, started at a media time against the system time (microseconds). Names and
// layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU58.h"
#include "types.h"

struct UnkMovieClock_b50 {
    nn::os::MutexType field_0x0;
    s64 field_0x20;  // the media time at the start
    s64 field_0x28;  // the system time at the start (-1: not started)
    s64 field_0x30;  // the media time it stops at
    s64 field_0x38;  // the earliest media time
};

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;  // the frame timer (as declared in src/base/input/UnkVibrationThread.cpp)

// Starts the clock: media time `time` was at system time `at`; false when either is negative or the
// start would fall before media time 0.
bool fn_710022073c(UnkMovieClock_b50* self, s64 time, s64 at, s64 limit) {
    if ((at | time) < 0)
        return false;
    fn_710006691c(&self->field_0x0);
    s64 now = fn_7100080b7c(g_unk_7101d47328);
    s64 start = now - at + time;
    bool ok;
    if (start >= 0) {
        self->field_0x20 = start;
        self->field_0x28 = now;
        self->field_0x30 = limit;
        ok = true;
    } else {
        ok = false;
    }
    fn_7100066920(&self->field_0x0);
    return ok;
}

// The system time at which media time `time` is due (left as is when the clock is not started); the
// current media time is held at the stop time unless `pastLimit`, and at the earliest time.
bool fn_71002207e8(UnkMovieClock_b50* self, s64 time, s64* out, bool pastLimit) {
    fn_710006691c(&self->field_0x0);
    if (self->field_0x28 != -1) {
        s64 now = fn_7100080b7c(g_unk_7101d47328);
        s64 current = now - self->field_0x28 + self->field_0x20;
        s64 stop = self->field_0x30;
        s64 earliest = self->field_0x38;
        if (current > stop)
            current = pastLimit ? current : stop;
        if (current < earliest)
            current = earliest;
        *out = now + time - (current < 0 ? 0 : current);
    }
    fn_7100066920(&self->field_0x0);
    return true;
}
