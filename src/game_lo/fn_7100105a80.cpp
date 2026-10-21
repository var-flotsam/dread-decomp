// Releases the pending callback object, then applies a queued request: either cancels a running
// countdown, or stores a new value (clamped to 10) / starts a fresh one through the helper.
#include "types.h"

struct UnkOwner_7100105a80;

class UnkCallback_7100105a80 {
public:
    virtual void vf_0(UnkOwner_7100105a80* owner) = 0;
};

struct UnkOwner_7100105a80 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    f32 field_0xc;
    u32 field_0x10;
    u8 pad_0x14[0x78 - 0x14];
    void* field_0x78;
    UnkCallback_7100105a80* field_0x80;
    u8 pad_0x88[0x100 - 0x88];
    f32 field_0x100;
    f32 field_0x104;
    u8 pad_0x108[0x110 - 0x108];
    f32 field_0x110;
    u8 pad_0x114[0x118 - 0x114];
    bool field_0x118;
    u8 pad_0x119[0x121 - 0x119];
    bool field_0x121;
    bool field_0x122;
};

void fn_71000ff8c4(void* p, f32 value, UnkOwner_7100105a80* owner);

void fn_7100105a80(UnkOwner_7100105a80* self, f32 value, f32 limit) {
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
