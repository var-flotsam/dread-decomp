// GUI::CHUD (unit 2751): one frame of the missile counter animation: advances its clock, counts the
// shown ammo from its start to its end value on the missile counter and, at the end, fires
// "OnMissileFill" with its sound; on its first frame "OnMissileRecover", and "OnAllDefault" once the
// count is reached. Names and layouts are placeholders.
#include <stdio.h>

#include "base/global/CRntString.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2747.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;


// The frame timer: the frame's length at +0x30.
struct UnkFrameTimer_4cb0 {
    u8 pad_0x0[0x30];
    f32 delta;  // 0x30
};

// A reference to a display object of the HUD's layout.
struct UnkHudRef_4cb0 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* target;  // 0x10
};

struct UnkHud_4cb0 {
    u8 pad_0x0[0x268];
    UnkHudRef_4cb0* missile_label;  // 0x268
    u8 pad_0x270[0x10];
    UnkHudRef_4cb0* missile_icon;  // 0x280
    UnkHudRef_4cb0* missile_frame;  // 0x288
    UnkHudRef_4cb0* missile_back;  // 0x290
};
static_assert(__builtin_offsetof(UnkHud_4cb0, missile_back) == 0x290, "UnkHud_4cb0 layout");

// The missile animation (built by fn_71011e3060).
struct UnkMissileAnim_4cb0 {
    void* vtable;
    UnkHud_4cb0* hud;  // 0x8
    f32 duration;      // 0x10
    f32 elapsed;       // 0x14
    f32 speed;         // 0x18
    bool done;         // 0x1c
    s32 from;          // 0x20
    s32 to;            // 0x24
    f32 shown;         // 0x28
};

static inline UnkGuiDisplayObject* Target(UnkHudRef_4cb0* ref) {
    return ref ? ref->target : nullptr;
}

// Plays the named animation event (vtable slot 0x1e8).
static inline void SendEvent(UnkGuiDisplayObject* item, const char* event) {
    using Fn = void (*)(UnkGuiDisplayObject*, const char*, bool);
    (*reinterpret_cast<Fn**>(item))[0x1e8 / 8](item, event, false);
}

void CHUD_StepMissileCounterAnimation(UnkMissileAnim_4cb0* self) {
    s32 count = self->to;
    f32 delta = reinterpret_cast<UnkFrameTimer_4cb0*>(g_unk_7101d47328)->delta;
    self->elapsed += delta * self->speed;
    if (self->elapsed <= self->duration) {
        f32 start = self->from;
        f32 range = count - self->from;
        self->shown = self->elapsed / self->duration * range + start;
        count = (s32)__builtin_roundf(self->shown);
    } else {
        self->done = true;
    }

    UnkGuiDisplayObject* label = Target(self->hud->missile_label);
    UnkGuiDisplayObject* icon = Target(self->hud->missile_icon);
    UnkGuiDisplayObject* frame = Target(self->hud->missile_frame);
    UnkGuiDisplayObject* back = Target(self->hud->missile_back);
    if (label && icon && frame) {
        fn_71011cdce8(label, false);
        fn_71011cdce8(icon, false);
        fn_71011cdce8(frame, false);
        if (count >= 0) {
            fn_71011cdce8(label, true);
            fn_71011cdce8(icon, true);
            fn_71011cdce8(frame, true);
            char text[16];
            sprintf(text, "%i / %i", count, self->to);
            fn_71011ec424(label, CRntString(text));
            label->vf_0x88();
            fn_71011cd5a0(label);
        }
    }

    if (self->done) {
        SendEvent(label, "OnMissileFill");
        SendEvent(icon, "OnMissileFill");
        SendEvent(frame, "OnMissileFill");
        SendEvent(back, "OnMissileFill");
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f02b0), self->hud);
        return;
    }
    if (self->to == self->from)
        return;
    const char* event;
    if (count == self->to)
        event = "OnAllDefault";
    else if (self->elapsed == delta * self->speed)
        event = "OnMissileRecover";
    else
        return;
    SendEvent(label, event);
    SendEvent(icon, event);
    SendEvent(frame, event);
    SendEvent(back, event);
}
