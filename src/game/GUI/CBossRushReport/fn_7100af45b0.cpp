// GUI::CBossRushReport: formats a signed number of seconds as "[+-]HH:MM:SS" wide text.
// Names and layouts are placeholders.
#include "types.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/timeline/CEvent.h"

struct UnkHms {
    u32 hours;
    u32 minutes;
    u32 seconds;
};

struct UnkWideStr_0x20 {
    u8 pad_0x0[0x20];
};
using UnkText = UnkWideStr_0x20;  // the GUI's name for it (src/game_s7/UnkGuiS7.h)

extern "C" int sprintf(char* buf, const char* fmt, ...);
void fn_7100081040(u32 total, UnkHms* out);

// NON_MATCHING(minor): the same instructions in another order or with other registers.
UnkText fn_7100af45b0(s32 seconds, bool showPlus) {
    UnkWideStr_0x20 result;
    UnkHms hms;
    char buf[0x80];
    s32 v = seconds < 359999 ? seconds : 359999;
    v = v > -359999 ? v : -359999;
    const char* sign = showPlus ? "+" : g_unk_710162e2ed;
    if (v < 0)
        sign = "-";
    fn_7100081040(v < 0 ? -v : v, &hms);
    sprintf(buf, "%s%02u:%02u:%02u", sign, hms.hours, hms.minutes, hms.seconds);
    fn_7100066b38(&result, buf);
    return result;
}
