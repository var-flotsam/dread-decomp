// Reset of a frame timer: clears the elapsed time at 0x20 and the scale pair at 0x30, then sets
// the three time stamps at 8, 0x10 and 0x18 to the current system time in microseconds. The
// same reset without the stamp at 8 is fn_7100080e38 (lifted). Its only caller is fn_7100015c70.

#include <nn/os/os_TickTypes.h>

#include "types.h"

struct UnkTimer_7100080c40 {
    u64 field_0x0;
    s64 start;
    s64 last;
    s64 current;
    s64 elapsed;
    u64 field_0x28;
    f32 field_0x30;
    f32 field_0x34;
};

// x / 1000 written out as the high half of a 64x64-bit product built from 32-bit halves, which is
// how the original computes it (a plain division compiles to smulh).
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

void fn_7100080c40(UnkTimer_7100080c40* self) {
    self->elapsed = 0;
    self->field_0x30 = 0.0f;
    self->field_0x34 = -1.0f;
    s64 now = DivideBy1000(nn::os::ConvertToTimeSpan(nn::os::GetSystemTick()).nanoseconds);
    self->last = self->current = now;
    self->start = now;
}
