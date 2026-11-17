// Unit 2732 (tools/tu_map.py): hands the captured controller states to every listener as button
// events, empties the capture table, and when nothing was pressed lets an animated camera know.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2730.h"
#include "types.h"

struct UnkFrameClock_l19b2fc {
    u8 pad_0x0[0x2c];
    f32 time;
};

struct UnkPadEvent_l19b2fc {
    u32 button;
    u32 type;  // 1 pressed, 2 held, 3 released
    f32 time;
    f32 value;
    u32 pad;
};

struct UnkPadListener_l19b2fc {
    u8 pad_0x0[0x30];
    u32 mask;
    bool alwaysListens;
    u8 storage[120 * sizeof(UnkPadEvent_l19b2fc)];  // the events, 120 at most
    u32 count;

    UnkPadEvent_l19b2fc* events() { return reinterpret_cast<UnkPadEvent_l19b2fc*>(storage); }
    s32 cooldown;

    void Push(u32 button, u32 type, f32 value, u32 pad) {
        if (count >= 120)
            return;
        f32 time = reinterpret_cast<UnkFrameClock_l19b2fc*>(g_unk_7101d47328)->time;
        events()[count].button = button;
        events()[count].type = type;
        events()[count].time = time;
        events()[count].value = value;
        events()[count].pad = pad;
        count++;
    }
};
static_assert(__builtin_offsetof(UnkPadListener_l19b2fc, count) == 0x998, "");

struct UnkPadListenerNode_l19b2fc {
    UnkPadListener_l19b2fc* listener;
    UnkPadListenerNode_l19b2fc* next;
};

struct UnkPadCapture_l19b2fc {
    u32 held;
    u32 released;
    u32 pressed;
    f32 values[32];
    u32 pad;
};

struct UnkPadRecorder_l19b2fc {
    u8 pad_0x0[0x10];
    UnkPadListenerNode_l19b2fc* listeners;
    u8 pad_0x18[0x18];
    UnkPadCapture_l19b2fc captures[60];
    u32 count;
    bool enabled;
    u8 pad_0x21f5[0xb];
    bool wasPaused;
};
static_assert(__builtin_offsetof(UnkPadRecorder_l19b2fc, wasPaused) == 0x2200, "");

void fn_710119b2fc(UnkPadRecorder_l19b2fc* self) {
    if (!self->enabled)
        return;
    bool paused = fn_7100decc5c(g_unk_7101d47310);
    bool any = false;
    for (u32 i = 0; i < self->count; i++) {
        UnkPadCapture_l19b2fc& capture = self->captures[i];
        // Any button state, or any analog value off centre, counts as input.
        if (!any) {
            any = capture.held || capture.released || capture.pressed;
            for (u32 k = 0; !any && k < 32; k++)
                any = capture.values[k] > 0.01f || capture.values[k] < -0.01f;
        }
        for (UnkPadListenerNode_l19b2fc* node = self->listeners; node; node = node->next) {
            if (!paused || node->listener->alwaysListens) {
                if (node->listener->cooldown > 0)
                    continue;
                for (u32 bit = 0; bit < 32; bit++) {
                    u32 button = 1u << bit;
                    if (!(node->listener->mask & button))
                        continue;
                    if (capture.pressed & button)
                        node->listener->Push(button, 1, capture.values[bit], capture.pad);
                    if (capture.held & button)
                        node->listener->Push(button, 2, capture.values[bit], capture.pad);
                    if (capture.released & button)
                        node->listener->Push(button, 3, capture.values[bit], capture.pad);
                }
            } else if (!self->wasPaused) {
                node->listener->Push(0x80000, 1, 0.0f, capture.pad);
            }
        }
    }
    self->wasPaused = paused;
    for (UnkPadListenerNode_l19b2fc* node = self->listeners; node; node = node->next) {
        s32 cooldown = node->listener->cooldown;
        node->listener->cooldown = cooldown > 0 ? cooldown - 1 : 0;
    }
    self->count = 0;
    if (any)
        return;
    if (fn_7101196fd8(g_unk_7101d47330))
        return;
    if (fn_7100ded930(g_unk_7101d47310))
        return;
    void* cameras = fn_7100defcec(g_unk_7101d47310);
    if (!cameras)
        return;
    UnkCameraCtrl* camera = fn_71010e2080(cameras);
    if (!camera)
        return;
    if (!fn_710009d50c(camera->vf_0x0(), fn_71010e7f90()))
        return;
    // Slot 0x60 of the animated camera controller.
    reinterpret_cast<void (*)(UnkCameraCtrl*)>(camera->vtable[12])(camera);
}
