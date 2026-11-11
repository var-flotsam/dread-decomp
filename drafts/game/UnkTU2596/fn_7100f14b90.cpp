// scratch
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include <stdio.h>
#include "types.h"

using base::global::CRntString;

struct UnkWideStr_0x20;
void fn_7100066b38(UnkWideStr_0x20* self, const char* str);
void* fn_7100deab70(UnkGameManager* self, const CRntString& name);
u32 fn_7100f14e80();
void fn_7100ec65fc(u8* self);
extern u8* g_unk_71726bc558;
f32 fn_7100ec73b4(u8* self);
f32 fn_7100ec73c4(u8* self);

// A text of 32-bit characters built from a string (fn_7100066b38).
struct UnkRewardText_b0242 {
    u8 raw[0x20];
    explicit UnkRewardText_b0242(const char* str) { fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(this), str); }
};

static inline u8* Timer() {
    u8* t = g_unk_71726bc558;
    if (!t) {
        t = static_cast<u8*>(fn_71000839a0(0x10, 0x3f));
        fn_7100ec65fc(t);
        g_unk_71726bc558 = t;
    }
    return t;
}

UnkRewardText_b0242 fn_7100f14db0() {
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CRntString name("REWARDS");
        fn_7100deab70(manager, name);
    }
    const char* text = g_unk_710162e2ed;
    if (fn_7100f14e80() <= 3) {
        Timer();
        fn_7100ec73b4(Timer());
        text = "#GUI_ENDING_CELEBRATION";
    }
    return UnkRewardText_b0242(text);
}

// Hours, minutes and seconds of a time in seconds, as "HHH:MM:SS" (capped at 999:59:59).
static inline void FormatTime(char* buf, f32 time) {
    u32 secs = (u32)time;
    u32 hours = secs / 3600;
    u32 rest = secs % 3600;
    u32 minutes = (u16)rest / 60;
    u16 seconds = rest - (u16)(minutes * 60);
    if (secs > 0x36ee7f) {
        hours = 999;
        minutes = 59;
        seconds = 59;
    }
    sprintf(buf, "%02u:%02u:%02u", hours, minutes, seconds);
}

UnkRewardText_b0242 fn_7100f14b90() {
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CRntString name("REWARDS");
        fn_7100deab70(manager, name);
    }
    char buf[0x80];
    FormatTime(buf, fn_7100ec73b4(Timer()));
    return UnkRewardText_b0242(buf);
}

UnkRewardText_b0242 fn_7100f14ca0() {
    char buf[0x80];
    if (Timer()[8]) {
        FormatTime(buf, fn_7100ec73c4(Timer()));
    } else {
        buf[0] = '-';
        buf[1] = 0;
    }
    return UnkRewardText_b0242(buf);
}
