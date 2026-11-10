// CShootActivatorHidrogigaComponent (unit of CCharClassShootActivatorHidrogigaComponent, tools/tu_map.py):
// switches the flag at +0x602 on or off. On a change it sets the fade (+0x604 from, +0x608 start, +0x60c target,
// +0x610 time 0.1, +0x614 elapsed) toward 1 when turned on (also marking +0x603) and toward 0 when turned off,
// then makes the virtual call in slot 62. Names and layouts are placeholders.
#include "types.h"

struct UnkHidrogigaActivator_ovb0036 {
    void** vtable;
    u8 pad_0x8[0x5fa];
    bool enabled;    // 0x602
    u8 field_0x603;  // 0x603
    f32 current;  // 0x604
    f32 from;     // 0x608
    f32 to;       // 0x60c
    f32 time;     // 0x610
    f32 elapsed;  // 0x614
};
static_assert(__builtin_offsetof(UnkHidrogigaActivator_ovb0036, elapsed) == 0x614, "layout");

void fn_7100898434(UnkHidrogigaActivator_ovb0036* self, bool enabled) {
    if (self->enabled == enabled)
        return;
    self->enabled = enabled;
    f32 from, to;
    if (!enabled) {
        self->current = 1.0f;
        from = 1.0f;
        to = 0.0f;
    } else {
        self->current = 0.0f;
        self->field_0x603 = 1;
        from = 0.0f;
        to = 1.0f;
    }
    self->time = 0.1f;
    self->elapsed = 0.0f;
    self->from = from;
    self->to = to;
    reinterpret_cast<void (*)(UnkHidrogigaActivator_ovb0036*)>(self->vtable[62])(self);
}
