// Unit 58: a frame timer's update: waits until the configured time has passed since the last update, taking the
// system time in microseconds, and scales the step. Names and layouts are placeholders.
#include <nn/os/os_TickTypes.h>

#include "game/UnkTU58.h"
#include "types.h"

struct UnkFrameTimer_b260 {
    u64 field_0x0;
    s64 start;      // 0x8
    s64 last;       // 0x10
    s64 current;    // 0x18
    s64 elapsed;    // 0x20
    u32 field_0x28;
    f32 total;      // 0x2c
    f32 step;       // 0x30
    f32 pending;    // 0x34
    s64 limit_ms;   // 0x38
    f32 divisor;    // 0x40
    f32 inverse;    // 0x44
    bool use_inverse;  // 0x48
};

static inline s64 DivideBy1000(s64 x) {
    const u64 lo = static_cast<u32>(x);
    const s64 hi = x >> 32;
    const u64 mlo = 0xe353f7cf;
    const u64 mhi = 0x20c49ba5;
    const s64 t = hi * static_cast<s64>(mlo) + static_cast<s64>((lo * mlo) >> 32);
    const s64 u = hi * static_cast<s64>(mhi);
    const u64 v = lo * mhi + static_cast<u32>(t);
    const s64 h = u + (t >> 32) + static_cast<s64>(v >> 32);
    return (h >> 7) + static_cast<s64>(static_cast<u64>(x) >> 63);
}

void fn_7100080cb8(UnkFrameTimer_b260* self, f32 scale) {
    self->inverse = 1.0f / self->divisor;
    self->last = self->current;
    self->elapsed = 0;
    if (self->limit_ms >= 0) {
        do {
            s64 now = DivideBy1000(nn::os::ConvertToTimeSpan(nn::os::GetSystemTick()).nanoseconds);
            self->current = now;
            self->elapsed = now - self->last;
            f32 cap = self->use_inverse ? self->inverse : 0.1f;
            if (self->pending > 0.0f) {
                self->step = self->pending;
                self->pending = -1.0f;
            } else {
                f32 seconds = (f32)self->elapsed * 0.000001f;
                if (seconds > cap)
                    seconds = cap;
                self->step = seconds * scale;
            }
        } while (!(self->elapsed / 1000 > self->limit_ms));
    }
    u32 old_total = *reinterpret_cast<u32*>(&self->total);
    self->field_0x28 = old_total;
    self->total = self->step + *reinterpret_cast<f32*>(&old_total);
    ++g_unk_7101d58e48;
}
