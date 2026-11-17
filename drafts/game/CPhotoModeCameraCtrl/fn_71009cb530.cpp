// CPhotoModeCameraCtrl's unit (tools/tu_map.py unit 1490): fn_71009cb530 consumes the queued input
// events of the photo mode camera, toggling the photo mode HUD and latching button and axis state.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_717273f970 __attribute__((visibility("hidden")));

struct UnkPhotoInputEvent_l9cb530 {
    u32 key;    // 0x0
    u32 type;   // 0x4
    u32 field_0x8;
    f32 value;  // 0xc
    u32 field_0x10;
};

struct UnkPhotoModeCameraCtrl_l9cb530 {
    u8 pad_0x0[0x248];
    UnkPhotoInputEvent_l9cb530 events[120];  // 0x248
    u32 event_count;                         // 0xba8
    u8 pad_0xbac[0x4];
    bool button_0x200;  // 0xbb0
    bool button_0x100;  // 0xbb1
    bool button_0x200000;  // 0xbb2
    bool button_0x100000;  // 0xbb3
    f32 axis_0x400000;   // 0xbb4
    f32 axis_0x800000;   // 0xbb8
    f32 axis_0x1000000;  // 0xbbc
    f32 axis_0x2000000;  // 0xbc0
    bool pressed;        // 0xbc4
    u8 pad_0xbc5[0x1b];
    bool hud_hidden;     // 0xbe0

    UnkPhotoInputEvent_l9cb530* Event(s32 index) { return &events[index]; }
};
static_assert(__builtin_offsetof(UnkPhotoModeCameraCtrl_l9cb530, hud_hidden) == 0xbe0, "layout");

void fn_71009cb530(UnkPhotoModeCameraCtrl_l9cb530* self) {
    self->button_0x200 = false;
    self->button_0x100 = false;
    self->button_0x200000 = false;
    self->button_0x100000 = false;
    self->axis_0x400000 = 0;
    self->axis_0x800000 = 0;
    self->axis_0x1000000 = 0;
    self->axis_0x2000000 = 0;
    self->pressed = false;

    u32 count = self->event_count;
    for (u32 i = 0; i < count; ++i) {
        UnkPhotoInputEvent_l9cb530* event = self->Event(i);
        if (event->type == 1) {
            if (event->key == 0x800 || event->key == 0x400) {
                self->hud_hidden = !self->hud_hidden;
                fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), self->hud_hidden,
                              &g_unk_717273f970);
                fn_7100219d4c(g_unk_7101d47300, self->hud_hidden, 2);
            }
            if (event->key == 1 || event->key == 2)
                self->pressed = true;
        }
        if (event->type == 2) {
            if (event->key == 0x100)
                self->button_0x100 = true;
            else if (event->key == 0x200)
                self->button_0x200 = true;
            if (event->key == 0x100000)
                self->button_0x100000 = true;
            else if (event->key == 0x200000)
                self->button_0x200000 = true;
        }
        if (event->key == 0x400000)
            self->axis_0x400000 = event->value;
        else if (event->key == 0x800000)
            self->axis_0x800000 = event->value;
        else if (event->key == 0x1000000)
            self->axis_0x1000000 = event->value;
        else if (event->key == 0x2000000)
            self->axis_0x2000000 = event->value;
    }
    self->event_count = 0;
}
