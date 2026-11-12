// CTimerComponent's unit (tools/tu_map.py unit 2723): the update (fn_7101182668). After the base update,
// a running timer with a period above FLT_EPSILON adds `dt` and, each time the elapsed time passes the
// period, sends its name (+0x50) to the owner's +0x1e0 object (fn_71009cde54), then stops (one-shot timers)
// or takes a period off. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CComponent.h"
#include "game/CPickableComponent.h"
#include "types.h"

struct UnkParent;

struct UnkTimerOwner_ovb0125 {
    u8 pad_0x0[0x1e0];
    void* field_0x1e0;
};

struct UnkTimerComponent_ovb0125 {
    u8 pad_0x0[0x30];
    UnkTimerOwner_ovb0125* owner;
    u8 pad_0x38[0x8];
    f32 period;   // 0x40
    f32 elapsed;  // 0x44
    bool once;     // 0x48
    bool running;  // 0x49
    u8 pad_0x4a[0x6];
    const char* name;  // 0x50
};

void fn_7101182668(UnkTimerComponent_ovb0125* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (self->running && self->period > 1.1920929e-07f) {
        self->elapsed += dt;
        if (self->elapsed > self->period) {
            void* target = self->owner->field_0x1e0;
            do {
                if (target && self->name && *self->name)
                    fn_71009cde54(target, self->name, g_unk_710162e2ed);
                if (self->once) {
                    if (self->running)
                        self->running = false;
                } else {
                    self->elapsed -= self->period;
                }
            } while (self->elapsed > self->period);
        }
    }
}
