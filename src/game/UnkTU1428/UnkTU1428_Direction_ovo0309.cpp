// CMorphBallMovement's unit: sets the movement direction and speed. When fn_710024fbc8 holds, copies the
// vector `a` to +0x40 and `b` to +0x58 and stores the speed at +0x7c; while the byte at +0xf9 is set,
// vf_0x3b0 answers or a's x is zero it keeps the sign of the value at +0x1d8 in step with a's x and
// raises the flags at +0x23d6, and with vf_0x3b0 true the direction becomes a unit step along x.
// Names and layouts are placeholders.
#include "game/CActorComponent.h"
#include "types.h"

struct UnkMorphBall_fd88 {
    void** vtable;
    u8 pad_0x8[0x40 - 0x8];
    u32 field_0x40[3];
    u8 pad_0x4c[0x58 - 0x4c];
    u32 field_0x58[3];
    u8 pad_0x64[0x7c - 0x64];
    f32 field_0x7c;
    u8 pad_0x80[0xf9 - 0x80];
    bool field_0xf9;
    u8 pad_0xfa[0x1d8 - 0xfa];
    f32 field_0x1d8;
    u8 pad_0x1dc[0x23c4 - 0x1dc];
    u32 field_0x23c4[2];
    u8 pad_0x23cc[0x23d6 - 0x23cc];
    bool field_0x23d6;
    u8 field_0x23d7[2];
    u8 pad_0x23d9[0x2658 - 0x23d9];
    f32 field_0x2658;

    bool vf_0x3b0() { return reinterpret_cast<bool (*)(UnkMorphBall_fd88*)>(vtable[0x3b0 / 8])(this); }
};

void fn_710093fd88(UnkMorphBall_fd88* self, const u32* a, const u32* b, f32 speed) {
    if (!fn_710024fbc8(self))
        return;
    bool flag = self->field_0xf9;
    bool moving = self->vf_0x3b0();
    u32 xbits = a[0];
    self->field_0x40[0] = xbits;
    self->field_0x40[1] = a[1];
    self->field_0x40[2] = a[2];
    if (flag) {
        if (moving) {
            self->field_0x40[0] = 0;
        } else {
            f32 x;
            __builtin_memcpy(&x, &xbits, 4);
            if ((x > 0.0f && self->field_0x1d8 < 0.0f) || (x < 0.0f && self->field_0x1d8 > 0.0f)) {
                if (self->field_0x2658 == 0.0f) {
                    if (x > 0.0f)
                        self->field_0x1d8 = __builtin_fabsf(self->field_0x1d8);
                    else if (x < 0.0f)
                        self->field_0x1d8 = -__builtin_fabsf(self->field_0x1d8);
                    if (!self->field_0x23d6 && self->field_0x23d7[1]) {
                        self->field_0x23d6 = true;
                        self->field_0x23c4[0] = 0;
                        self->field_0x23c4[1] = 0;
                        self->field_0x23d7[0] = 0;
                        self->field_0x23d7[1] = 0;
                    }
                }
            }
        }
    }
    self->field_0x58[0] = b[0];
    self->field_0x58[1] = b[1];
    self->field_0x58[2] = b[2];
    if (flag && moving) {
        if (self->field_0x1d8 > 0.0f) {
            u64 xy = 0x3f800000;
            __builtin_memcpy(&self->field_0x58[0], &xy, 8);
            self->field_0x58[2] = 0;
        } else if (self->field_0x1d8 < 0.0f) {
            u64 xy = 0xbf800000;
            __builtin_memcpy(&self->field_0x58[0], &xy, 8);
            self->field_0x58[2] = 0;
        }
    }
    self->field_0x7c = speed;
}
