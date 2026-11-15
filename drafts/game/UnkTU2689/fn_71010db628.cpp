// Unit 2689 (CPlayerMovement hint): turns the movement to follow the yaw of the shape behind +0x128 and, when
// the shape's part at +0x158 answers 2 from its vf_0x18, sets the velocity at +0x1e4 that reaches the offset
// point in dt; returns the yaw change. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

struct UnkPart_l0db628 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 field_0xa8;

    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkPart_l0db628*)>(vtable[0x18 / 8])(this); }
};

struct UnkShape {
    u8 pad_0x0[0x158];
    UnkPart_l0db628* field_0x158;
};

struct UnkHolder_l0db628 {
    u8 pad_0x0[0x10];
    UnkShape* field_0x10;
};

struct UnkLink_l0db628 {
    UnkHolder_l0db628* holder;
    u8 pad_0x8[0xc];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0xc];
    f32 field_0x2c;
};

struct UnkMovement_l0db628 {
    u8 pad_0x0[0x128];
    UnkLink_l0db628 link;  // +0x128
    u8 pad_0x130[0x158 - 0x128 - sizeof(UnkLink_l0db628)];
    f32 field_0x158;
    u8 pad_0x15c[0x1e4 - 0x15c];
    f32 velocity[3];  // +0x1e4
    u8 pad_0x1f0[0x6f8 - 0x1f0];
    f32 yaw;          // +0x6f8
    f32 field_0x6fc;
};

f32 fn_71010db628(UnkMovement_l0db628* self, f32 dt) {
    f32 delta = 0.0f;
    if (self->link.holder && self->link.holder->field_0x10) {
        UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->link.holder->field_0x10));
        f32 newYaw = atan2f(m.m[5], m.m[1]);
        delta = newYaw - self->yaw;
        self->yaw = newYaw;
        f32 x = self->link.field_0x2c;
        f32 angle = delta + atan2f(self->field_0x158, x);
        f32 c = cosf(angle);
        f32 s = sinf(angle);
        UnkPart_l0db628* part = self->link.holder->field_0x10->field_0x158;
        // The original asks the part's vf_0x18 before it tests the part for null.
        s32 kind = part->vf_0x18();
        if (part && kind == 2) {
            self->field_0x6fc = part->field_0xa8;
            f32 inv = 1.0f / dt;
            self->velocity[0] = inv * (m.m[3] + c * part->field_0xa8 - self->link.field_0x14);
            self->velocity[1] = inv * (m.m[7] + s * part->field_0xa8 - self->link.field_0x18);
            self->velocity[2] = inv * (m.m[11] + part->field_0xa8 * 0.0f - self->link.field_0x1c);
            return delta;
        }
    }
    return 0.0f;
}
