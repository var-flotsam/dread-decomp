// Unit 1246's functions (tools/tu_map.py hint "CEvacuationCountDown"): the countdown's staged
// timeline keys, each played once when the elapsed share of the total passes its threshold, and the
// remaining-time label ("mm:ss") with the events at the three remaining-time marks. Names and
// layouts are placeholders.
#include "UnkLeftoverReg.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU57.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntString;

// The tunable block (the total time, the three thresholds, the remaining-time marks).
struct UnkCountDownTunables_ovb0259 {
    u8 pad_0x0[0x38];
    f32 total;  // 0x38
    u8 pad_0x3c[0x14];
    f32 stage_0x50;
    f32 stage_0x54;
    f32 stage_0x58;
    s32 first;   // 0x5c
    s32 second;  // 0x60
    s32 third;   // 0x64
};
extern UnkCountDownTunables_ovb0259 g_unk_717271e848;

extern const u64 g_unk_710164ab28;
extern const u64 g_unk_710164ab30;
extern const u64 g_unk_710164ab38;

struct UnkCountDownGui_ovb0259 {
    virtual const base::reflection::CClass* vf_0x0();
    void SendEvent(const char* name, bool now) {
        reinterpret_cast<void (*)(UnkCountDownGui_ovb0259*, const char*, bool)>(
            (*reinterpret_cast<void***>(this))[0x1e8 / 8])(this, name, now);
    }
};

struct UnkCountDownGuiHolder_ovb0259 {
    u8 pad_0x0[0x10];
    UnkCountDownGui_ovb0259* gui;
};

struct UnkCountDown_ovb0259 {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x48];
    UnkCountDownGuiHolder_ovb0259* field_0x80;
    u8 pad_0x88[0x38];
    f32 elapsed;  // 0xc0
    bool field_0xc4;
    bool field_0xc5;
    bool field_0xc6;
};

// Its first parameter is an empty object (passed in x0 with nothing set).
struct UnkCountDownEmpty_ovb0259 : UnkLeftoverReg {};
void fn_71007da43c(UnkCountDownEmpty_ovb0259 tag, u32 a, u32 b);

void fn_71007daf04(UnkCountDown_ovb0259* self) {
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    f32 total = t->total;
    if (total + 0.00001f >= 0.0f && total - 0.00001f <= 0.0f)
        return;
    UnkCountDownEmpty_ovb0259 s;
    f32 share = self->elapsed * 100.0f / total;
    if (!self->field_0xc4 && share >= t->stage_0x50) {
        fn_7100d7c828(self->field_0x30, &g_unk_710164ab28, false);
        self->field_0xc4 = true;
        fn_71007da43c(s, 0, 2);
        return;
    }
    if (!self->field_0xc5 && share >= t->stage_0x54) {
        fn_7100d7c828(self->field_0x30, &g_unk_710164ab30, false);
        self->field_0xc5 = true;
        fn_71007da43c(s, 0, 0);
        fn_71007da43c(s, 1, 2);
        return;
    }
    if (!self->field_0xc6 && share >= t->stage_0x58) {
        fn_7100d7c828(self->field_0x30, &g_unk_710164ab38, false);
        self->field_0xc6 = true;
        fn_71007da43c(s, 1, 0);
        fn_71007da43c(s, 2, 2);
    }
}

struct UnkGuiObj;
UnkGuiObj* fn_71011d9508(UnkGuiObj*, const char* const*);
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);
void fn_71011cd5a0(UnkGuiObj*);

void fn_71007da730(UnkCountDown_ovb0259* self) {
    UnkCountDownGui_ovb0259* holder = self->field_0x80 ? self->field_0x80->gui : nullptr;
    UnkGuiObj* label;
    {
        CRntString key("Counter.Content.Label");
        label = fn_71011d9508(reinterpret_cast<UnkGuiObj*>(holder), reinterpret_cast<const char* const*>(&key));
        if (label) {
            const base::reflection::CClass* cls = reinterpret_cast<UnkCountDownGui_ovb0259*>(label)->vf_0x0();
            if (!fn_710009d50c(cls, fn_71011e8dfc()))
                label = nullptr;
        }
    }
    if (!label)
        return;
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    s32 remaining = (s32)(t->total - self->elapsed);
    if (t->first >= remaining && t->second < remaining)
        self->field_0x80->gui->SendEvent("On60Remaining", false);
    else if (t->second >= remaining && t->third < remaining)
        self->field_0x80->gui->SendEvent("On30Remaining", false);
    else if (t->third >= remaining)
        self->field_0x80->gui->SendEvent("On15Remaining", false);
    base::UnkNamedEntry entry;
    fn_710007e6c4(&entry);
    CRntString& text = *reinterpret_cast<CRntString*>(&entry);
    s32 seconds;
    if (remaining >= 60) {
        u32 minutes = static_cast<u32>(remaining) / 60;
        seconds = remaining - 60 * minutes;
        if (remaining <= 599)
            fn_710007f564(&text, "0");
        fn_710007f634(&text, minutes);
        fn_710007f564(&text, ":");
    } else {
        seconds = remaining;
        fn_710007f564(&text, "00:");
    }
    if (seconds <= 9)
        fn_710007f564(&text, "0");
    fn_710007f634(&text, seconds);
    const char* chars = text.data;
    fn_71011ec424(label, CRntString(chars ? chars : g_unk_710162e2ed));
    fn_71011cd5a0(label);
    fn_710007ecd4(&text);
}
