// Unit 1246's functions (tools/tu_map.py hint "CEvacuationCountDown"): the countdown's staged
// timeline keys, each played once when the elapsed share of the total passes its threshold, and the
// remaining-time label ("mm:ss") with the events at the three remaining-time marks. Names and
// layouts are placeholders.
#include "UnkLeftoverReg.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU315.h"
#include "game/UnkTU53.h"
#include "game/UnkTU57.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntString;

// The tunable block: the curves and ranges of the camera and screen effects, the total time, the three
// stage thresholds and the remaining-time marks.
struct UnkCountDownTunables_ovb0259 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_0x2c[0x4];
    base::global::CStrId curve_0x30;
    f32 total;  // 0x38
    f32 field_0x3c;
    f32 field_0x40;
    u8 pad_0x44[0x4];
    base::global::CStrId curve_0x48;
    f32 stage_0x50;
    f32 stage_0x54;
    f32 stage_0x58;
    s32 first;   // 0x5c
    s32 second;  // 0x60
    s32 third;   // 0x64
    u8 pad_0x68[0x30];
    f32 field_0x98;
    f32 field_0x9c;
    base::global::CStrId curve_0xa0;
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
    u8 pad_0x38[0x41];
    bool field_0x79;
    u8 pad_0x7a[0x6];
    UnkCountDownGuiHolder_ovb0259* field_0x80;
    u8 pad_0x88[0x38];
    f32 elapsed;  // 0xc0
    bool field_0xc4;
    bool field_0xc5;
    bool field_0xc6;
    u8 pad_0xc7[0x29];
    bool field_0xf0;
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

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj*, const char* const*);
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);

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

struct UnkInputComponent;
struct UnkZeroVec_ovb0259 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    // Zero, each member set in turn (an aggregate's zeroing is stored in another order).
    UnkZeroVec_ovb0259() {
        field_0x0 = 0;
        field_0x4 = 0;
        field_0x8 = 0;
    }
};
struct UnkPlayer;
struct UnkScreenFadeState_b48a3;
bool fn_7101053a1c(UnkScreenFadeState_b48a3* self, f32 t);
base::reflection::UnkReflObject* fn_7100e0398c(UnkGameManager*, void*);
struct UnkMapOwner;
struct UnkKeyObject;
bool fn_7100e039e4(UnkMapOwner* self, UnkKeyObject* const* key);
extern char g_unk_717271e920[] __attribute__((visibility("hidden")));
extern char g_unk_717271e930[] __attribute__((visibility("hidden")));

// Whether the game manager's map holds the key object at `key`.
static inline bool HasKey_l7dacbc(char* key) {
    return fn_7100e039e4(reinterpret_cast<UnkMapOwner*>(g_unk_7101d47310), reinterpret_cast<UnkKeyObject* const*>(key));
}

struct UnkPlayerInput_ovb0259 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* field_0x1d8;
};

struct UnkShake_ovb0259 {
    u8 pad_0x0[0x60];
    f32 field_0x60;
};

static inline f32 LerpClamped(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

static inline void* Block() { return reinterpret_cast<u8*>(g_unk_7101d47310) + 0x1040; }

static inline void* Manager() {
    UnkManager* instance = g_unk_7101d47360;
    if (!instance) {
        instance = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(instance);
        g_unk_7101d47360 = instance;
    }
    return instance;
}

void fn_71007db2b0(UnkCountDown_ovb0259* self);
void fn_71007dacbc(UnkCountDown_ovb0259* self) {
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    f32 ratio = self->elapsed / t->total;
    f32 clamped = ratio <= 0.0f ? 0.0f : (ratio >= 1.0f ? 1.0f : ratio);
    f32 v = fn_7100076020(clamped, t->curve_0x48.Text());
    f32 value = LerpClamped(t->field_0x3c, t->field_0x40, v);
    if (self->elapsed > t->total) {
        UnkPlayerInput_ovb0259* player = reinterpret_cast<UnkPlayerInput_ovb0259*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->field_0x1d8 && fn_710087238c(player->field_0x1d8))
            goto done;
        if (fn_7100eef674(Manager()))
            goto done;
        if (fn_7101053a0c(Block()))
            goto done;
        if (fn_7101053a98(Block()))
            goto done;
        if (fn_7101053a7c(Block()))
            goto done;
        if (!fn_7101053a1c(static_cast<UnkScreenFadeState_b48a3*>(Block()), 0.0f))
            goto next;
    }
done:
    if (base::reflection::UnkReflObject* fx = fn_7100e0398c(g_unk_7101d47310, g_unk_717271e920)) {
        if (fn_710009d50c(fx->vf_0x0(), fn_71010f21a4())) {
            if (!HasKey_l7dacbc(g_unk_717271e920)) {
                UnkZeroVec_ovb0259 zero;
                fn_7100e038b0(g_unk_7101d47310, g_unk_717271e920, reinterpret_cast<UnkVec*>(&zero), 0);
            }
            reinterpret_cast<UnkShake_ovb0259*>(fx)->field_0x60 = value;
        }
    }
    return;
next:
    if (!HasKey_l7dacbc(g_unk_717271e930)) {
        UnkZeroVec_ovb0259 zero;
        fn_7100e038b0(g_unk_7101d47310, g_unk_717271e930, reinterpret_cast<UnkVec*>(&zero), 0);
    }
    fn_7100e03a78(g_unk_7101d47310, g_unk_717271e920);
    fn_71007db2b0(self);
}

struct UnkParent;
extern const u64 g_unk_710164ab48;

void fn_71007daa8c(UnkCountDown_ovb0259* self);
void fn_71007db054(UnkCountDown_ovb0259* self, f32 dt);

struct UnkPlayerPosition_ovb0259 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkTargetView_ovb0259 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x348];
    u8 field_0x3b0;
};

static inline f32 CurveAt(UnkCountDownTunables_ovb0259* t, f32 ratio) {
    f32 clamped = ratio <= 0.0f ? 0.0f : (ratio >= 1.0f ? 1.0f : ratio);
    return fn_7100076020(clamped, t->curve_0xa0.Text());
}

void fn_71007da92c(UnkCountDown_ovb0259* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkPlayerPosition_ovb0259* player = reinterpret_cast<UnkPlayerPosition_ovb0259*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        UnkTargetView_ovb0259* target = reinterpret_cast<UnkTargetView_ovb0259*>(self->field_0x30);
        u32 a = player->field_0x44;
        u32 b = player->field_0x54;
        u32 c = player->field_0x64;
        target->field_0x44 = a;
        target->field_0x54 = b;
        target->field_0x64 = c;
        target->field_0x3b0 = 1;
    }
    if (!self->field_0x79 || !self->field_0xf0)
        return;
    fn_71007daa8c(self);
    fn_71007dacbc(self);
    fn_71007daf04(self);
    fn_71007da730(self);
    fn_71007db054(self, dt);
    UnkCountDownTunables_ovb0259* t = &g_unk_717271e848;
    f32 ratio = self->elapsed / t->total;
    f32 v = CurveAt(t, ratio);
    f32 value = LerpClamped(t->field_0x98, t->field_0x9c, v);
    base::math::CVector4D vec;
    vec.x = value;
    vec.y = 0.0f;
    vec.z = 0.0f;
    vec.w = 0.0f;
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164ab48, &vec);
    self->elapsed = self->elapsed + dt;
}
