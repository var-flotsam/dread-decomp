// GUI::CBossRushReport: formats a signed number of seconds as "[+-]HH:MM:SS" wide text.
// Names and layouts are placeholders.
#include "types.h"
#include "base/global/CFilePathStrId.h"

struct UnkHms {
    u32 hours;
    u32 minutes;
    u32 seconds;
};

struct UnkWideStr_0x20 {
    u8 pad_0x0[0x20];
};

extern "C" int sprintf(char* buf, const char* fmt, ...);
void fn_7100081040(u32 total, UnkHms* out);
void fn_7100066b38(UnkWideStr_0x20* self, const char* str);

UnkWideStr_0x20 fn_7100af45b0(s32 seconds, bool showPlus) {
    UnkWideStr_0x20 result;
    UnkHms hms;
    char buf[0x80];
    s32 v = seconds < 359999 ? seconds : 359999;
    s32 lo = -359999;
    asm("" : "+r"(lo) : "r"(v));
    v = v > lo ? v : lo;
    const char* sign = showPlus ? "+" : g_unk_710162e2ed;
    if (v < 0)
        sign = "-";
    fn_7100081040(v < 0 ? -v : v, &hms);
    sprintf(buf, "%s%02u:%02u:%02u", sign, hms.hours, hms.minutes, hms.seconds);
    fn_7100066b38(&result, buf);
    return result;
}
