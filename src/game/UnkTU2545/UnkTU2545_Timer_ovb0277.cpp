// Unit 2545 of tools/tu_map.py: a timer panel (see UnkTU2545_Counter_ovb0232.cpp): its time text and hiding
// its children, and its per-frame input poll. Names and layouts are placeholders.
#include <stdio.h>
#include <new>

#include "base/UnkVector.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/nn/time.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkHms {
    u32 hours;
    u32 minutes;
    u32 seconds;
};
void fn_7100081040(u32 total, UnkHms* out);

struct UnkWideStr_0x20;
using UnkGuiObj = UnkGuiDisplayObject;
void fn_71011cfba8(UnkGuiObj* object, f32 alpha);

struct UnkTimeText_b0277 {
    UnkGuiText text;
    explicit UnkTimeText_b0277(const char* str) { fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(this), str); }
    ~UnkTimeText_b0277() { fn_7100066e44(this); }
};

struct UnkTimerPanel_b0277 {
    u8 pad_0x0[0xbc];
    bool field_0xbc;
    u8 pad_0xbd[3];
    f32 field_0xc0;
    u8 pad_0xc4[4];
    f32 field_0xc8;
    u8 pad_0xcc[0x14];
    bool field_0xe0;
    u8 pad_0xe1[0x33];
    f32 field_0x114;
    f32 field_0x118;
    f32 field_0x11c;
    u8 pad_0x120[0x28];
    UnkGuiDisplayObject* field_0x148;
    u8 pad_0x150[0x48];
    UnkGuiDisplayObject* field_0x198;
    UnkGuiDisplayObject* field_0x1a0;
    u8 pad_0x1a8[0x18];
    UnkGuiDisplayObject* field_0x1c0;
    UnkGuiDisplayObject* field_0x1c8;
    u8 pad_0x1d0[0x18];
    UnkGuiDisplayObject* field_0x1e8;
    UnkGuiDisplayObject* field_0x1f0;
    u8 pad_0x1f8[0x18];
    UnkGuiDisplayObject* field_0x210;
    UnkGuiDisplayObject* field_0x218;
    u8 pad_0x220[0x18];
    UnkGuiDisplayObject* field_0x238;
    UnkGuiDisplayObject* field_0x240;
};
static_assert(__builtin_offsetof(UnkTimerPanel_b0277, field_0x240) == 0x240, "layout");

#define HIDE(obj)                  \
    if (self->obj)                 \
        fn_71011cfba8(self->obj, 0.0f)

void fn_7100dd7750(UnkTimerPanel_b0277* self) {
    f32 total = self->field_0xc8;
    if (self->field_0xc0 > 0.0f && !self->field_0xe0 && !self->field_0xbc)
        total = self->field_0xc8 + self->field_0xc0;
    if (self->field_0x114 > 0.0f)
        total += self->field_0x114;
    if (self->field_0x11c > 0.0f)
        total = self->field_0x11c + total;
    if (self->field_0x118 > 0.0f)
        total = self->field_0x118 + total;
    u32 seconds = (u32)total;
    if (seconds > 359999)
        seconds = 359999;
    UnkHms hms;
    fn_7100081040(seconds, &hms);
    char buf[0x80];
    sprintf(buf, "%02u:%02u:%02u", hms.hours, hms.minutes, hms.seconds);
    UnkTimeText_b0277 text(buf);
    fn_71011cdce8(self->field_0x148, false);
    fn_71011cdc30(self->field_0x148, false);
    fn_71011ec468(self->field_0x148, text.text, false);
    fn_71011cdce8(self->field_0x148, true);
    fn_71011cdc30(self->field_0x148, true);
    HIDE(field_0x1a0);
    HIDE(field_0x198);
    HIDE(field_0x1c8);
    HIDE(field_0x1c0);
    HIDE(field_0x1f0);
    HIDE(field_0x1e8);
    HIDE(field_0x218);
    HIDE(field_0x210);
    HIDE(field_0x240);
    HIDE(field_0x238);
}
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
inline const UnkLoadingMask& UnkMaskNone4() {
    static UnkLoadingMask s asm("g_unk_71727bdf70");
    return s;
}
inline const UnkLoadingMask& UnkMaskAll4() {
    static UnkLoadingMask s asm("g_unk_71727bdf60")(~UnkMaskNone4().bits & 0xf);
    return s;
}
inline const UnkLoadingMask& UnkMaskNone9() {
    static UnkLoadingMask s asm("g_unk_71726c4090");
    return s;
}
inline const UnkLoadingMask& UnkMaskAll9() {
    static UnkLoadingMask s asm("g_unk_71726c4080")(~UnkMaskNone9().bits & 0x1ff);
    return s;
}
extern UnkMgr_0xf18* g_unk_71726bc560;

struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;

struct UnkInput_b0277 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual bool IsPressed(u32 button, u32 mask);
};

static inline UnkSoundHolder* SoundHolder_b0277() {
    if (!g_unk_7101d47340) {
        auto* h = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(h);
        g_unk_7101d47340 = h;
    }
    return g_unk_7101d47340;
}

struct UnkPanel_b0277 {
    u8 pad_0x0[0x269];
    bool field_0x269;
};

void fn_7100dd60fc(UnkPanel_b0277* self) {
    void* music = reinterpret_cast<void*&>(g_unk_71726bc560);
    if (!music) {
        music = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(music);
        reinterpret_cast<void*&>(g_unk_71726bc560) = music;
    }
    fn_7100fd7b44(music, UnkMaskAll4(), UnkMaskAll9(), 0.0f);
    if (self->field_0x269)
        return;
    auto* input = *reinterpret_cast<UnkInput_b0277**>(SoundHolder_b0277());
    if (input->IsPressed(1, 0xfd) || input->IsPressed(0x400, 0xfd) || input->IsPressed(0x800, 0xfd) ||
        input->IsPressed(2, 0xfd))
        self->field_0x269 = true;
}
