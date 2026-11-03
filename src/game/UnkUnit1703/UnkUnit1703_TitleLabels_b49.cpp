// Unit 1703 (tools/tu_map.py, no name hint): two updates of a title screen with a "PressALabel", a
// "VersionLabel" and a "NintendoLabel" under its root (+0x8). Each counts a delay down by the frame
// time and, once it has run out (the flag after it), changes the labels through their vf_0x78.
// fn_7100b53b44 (delay at +0x20): a pending notice (+0x25) goes to the target (fn_7100b36770), the
// screen steps on (fn_7100b53fa0) and the labels get -140 (with 0.5 at +0x28). fn_7100b53d58 (delay at
// +0x18): the labels get 0, the screen finishes (fn_7100b541d0) and the main menu (when it is one,
// fn_7100b34230) is told the word at +0x14 (fn_7100b35704). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

UnkReflObject* fn_7100e033a8(UnkGameManager* manager);

struct UnkTitleTarget;
void fn_7100b36770(UnkTitleTarget* target);
void fn_7100b35704(UnkReflObject* menu, u32 value, bool flag);

static inline void SetLabel(UnkGuiDisplayObject* label, s32 value) {
    reinterpret_cast<void (*)(UnkGuiDisplayObject*, s32, bool)>((*reinterpret_cast<void***>(label))[0x78 / 8])(
        label, value, false);
}

struct UnkTitleScreenA {
    void* vtable;
    UnkGuiDisplayObject* root;  // 0x8
    u8 pad_0x10[0x10];
    f32 delay;  // 0x20
    bool done;  // 0x24
    bool notify;
    u8 pad_0x26[2];
    f32 field_0x28;
};

struct UnkTitleScreenB {
    void* vtable;
    UnkGuiDisplayObject* root;  // 0x8
    u8 pad_0x10[4];
    u32 field_0x14;
    f32 delay;  // 0x18
    bool done;  // 0x1c
};

void fn_7100b53fa0(UnkTitleScreenA* self);
void fn_7100b541d0(UnkTitleScreenB* self);

template <class T>
__attribute__((always_inline)) static inline bool CountDown(T* self, f32 dt) {
    if (self->delay > 0.0f) {
        self->delay -= dt;
        if (self->delay <= 0.0f)
            self->done = true;
    }
    return self->done;
}

void fn_7100b53b44(UnkTitleScreenA* self, UnkTitleTarget* target, f32 dt) {
    UnkGuiDisplayObject* press = fn_71011d94d4(self->root, CStrId("PressALabel", true));
    UnkGuiDisplayObject* version = fn_71011d94d4(self->root, CStrId("VersionLabel", true));
    UnkGuiDisplayObject* nintendo = fn_71011d94d4(self->root, CStrId("NintendoLabel", true));
    if (!CountDown(self, dt))
        return;
    if (self->notify) {
        self->notify = false;
        fn_7100b36770(target);
    }
    fn_7100b53fa0(self);
    if (press) {
        self->field_0x28 = 0.5f;
        SetLabel(press, -140);
    }
    if (version)
        SetLabel(version, -140);
    if (nintendo)
        SetLabel(nintendo, -140);
}

void fn_7100b53d58(UnkTitleScreenB* self, f32 dt) {
    UnkGuiDisplayObject* press = fn_71011d94d4(self->root, CStrId("PressALabel", true));
    UnkGuiDisplayObject* version = fn_71011d94d4(self->root, CStrId("VersionLabel", true));
    UnkGuiDisplayObject* nintendo = fn_71011d94d4(self->root, CStrId("NintendoLabel", true));
    if (!CountDown(self, dt))
        return;
    if (press)
        SetLabel(press, 0);
    if (version)
        SetLabel(version, 0);
    if (nintendo)
        SetLabel(nintendo, 0);
    fn_7100b541d0(self);
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    fn_7100b35704(menu && fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr, self->field_0x14, true);
}

// fn_7100b52a80: on entering the title screen (fn_7100b52f04 first) the "PressALabel" and
// "NintendoLabel" get 0 and are switched off (fn_71011cdce8), the "VersionLabel" gets -140.
void fn_7100b52f04(void* self);

void fn_7100b52a80(UnkTitleScreenA* self) {
    UnkGuiDisplayObject* press = fn_71011d94d4(self->root, CStrId("PressALabel", true));
    UnkGuiDisplayObject* version = fn_71011d94d4(self->root, CStrId("VersionLabel", true));
    UnkGuiDisplayObject* nintendo = fn_71011d94d4(self->root, CStrId("NintendoLabel", true));
    fn_7100b52f04(self);
    if (press) {
        SetLabel(press, 0);
        fn_71011cdce8(press, false);
    }
    if (nintendo) {
        SetLabel(nintendo, 0);
        fn_71011cdce8(nintendo, false);
    }
    if (version)
        SetLabel(version, -140);
}
