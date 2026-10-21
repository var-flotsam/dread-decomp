// Time helpers.
#include "types.h"

struct UnkHms {
    u32 hours;
    u32 minutes;
    u32 seconds;
};

// Splits a number of seconds into hours, minutes and seconds, capped at 999:59:59.
void fn_7100081040(u32 total, UnkHms* out) {
    u16 rem = total % 3600;
    u16 minutes = rem / 60;
    out->hours = total / 3600;
    out->minutes = minutes;
    out->seconds = (u16)(rem % 60);
    if (total >= 3600000) {
        out->hours = 999;
        out->minutes = 59;
        out->seconds = 59;
    }
}
