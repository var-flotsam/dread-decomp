// Releases the pending callback object, then applies a queued request: either cancels a running
// countdown, or stores a new value (clamped to 10) / starts a fresh one through the helper.
#include "base/snd/UnkSound.h"
#include "types.h"

void fn_71000ff8c4(void* p, f32 value, UnkSound* owner);

void fn_7100105a80(UnkSound* self, f32 value, f32 limit) {
    if (self->field_0x80) {
        self->field_0x80->vf_0(self);
        self->field_0x80 = nullptr;
    }
    if (!self->field_0x118)
        return;
    self->field_0x122 = true;
    self->field_0x118 = false;
    if (self->field_0x110 > 0.0f) {
        self->field_0x110 = -1.0f;
        self->field_0x121 = true;
        return;
    }
    if (value < 0.0f)
        value = self->field_0x100;
    if (limit <= 0.0f) {
        self->field_0x104 = 0.0f;
        fn_71000ff8c4(self->field_0x78, value, self);
    } else if (limit >= 10.0f) {
        self->field_0x104 = 10.0f;
    } else {
        self->field_0x104 = limit;
    }
    f32 remaining = self->field_0x104;
    self->field_0x100 = value;
    if (value > 0.0f && remaining <= 0.0f) {
        self->field_0x4 = self->field_0x0;
        self->field_0x8 = 0;
        self->field_0xc = value;
        self->field_0x10 = 0;
    }
}
