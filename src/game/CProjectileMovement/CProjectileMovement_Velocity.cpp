// CProjectileMovement (unit 1623 of tools/tu_map.py, the name from reflection): the velocity is the
// direction (+0x40) times the speed (virtual slot 0x2f8) scaled by +0x7c, stored at +0x4b8. When
// the object at +0x5c8 has its flag (+0x130) set, the speed and the direction's absolute y and x
// are also kept (+0x5c0, +0x5b8, +0x5bc). Names and layouts are placeholders.
#include "types.h"

struct UnkProjectileState {
    u8 unk_0x0[0x130];
    bool flag;
};

struct UnkProjectileMovement {
    void** vtable;
    u8 unk_0x8[0x38];
    f32 dir_x;  // 0x40
    f32 dir_y;
    f32 dir_z;
    u8 unk_0x4c[0x30];
    f32 scale;  // 0x7c
    u8 unk_0x80[0x438];
    f32 vel_x;  // 0x4b8
    f32 vel_y;
    f32 vel_z;
    u8 unk_0x4c4[0xf4];
    f32 abs_y;  // 0x5b8
    f32 abs_x;
    f32 speed;  // 0x5c0
    u8 unk_0x5c4[0x4];
    UnkProjectileState* state;  // 0x5c8

    f32 vf_0x2f8() { return reinterpret_cast<f32 (*)(UnkProjectileMovement*)>(vtable[0x2f8 / 8])(this); }
};

void fn_7100ac64cc(UnkProjectileMovement* self) {
    f32 speed = self->vf_0x2f8() * self->scale;
    f32 x = self->dir_x;
    f32 y = self->dir_y;
    f32 z = self->dir_z;
    f32 vx = x * speed;
    f32 vy = speed * y;
    f32 vz = speed * z;
    self->vel_x = vx;
    self->vel_y = vy;
    self->vel_z = vz;
    if (self->state && self->state->flag) {
        self->speed = speed;
        self->abs_y = __builtin_fabsf(y);
        self->abs_x = __builtin_fabsf(x);
    }
}
