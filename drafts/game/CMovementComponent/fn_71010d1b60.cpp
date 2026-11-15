// CMovementComponent unit: per-frame step that remembers the previous values (vectors +0x40 and +0x58, a
// word at +0x7c, the flag at +0xf9), damps the velocity (+0x1d8) by the drag (+0x5c of the object at +0x1f8)
// over dt, and times how long the flag has been set (+0x200) or clear (+0x204). Names and layouts are
// placeholders.
#include "types.h"

struct UnkDragSource_l0d1b60 {
    u8 pad_0x0[0x5c];
    f32 drag;  // 0x5c
};

struct UnkDragMovement_l0d1b60;

struct UnkDragMovementVt_l0d1b60 {
    u8 pad_0x0[0x260];
    void (*on_set)(UnkDragMovement_l0d1b60*);  // 0x260
};

struct UnkDragVec_l0d1b60 {
    f32 x, y, z;
};

struct UnkDragMovement_l0d1b60 {
    UnkDragMovementVt_l0d1b60* vt;
    u8 pad_0x8[0x38];
    UnkDragVec_l0d1b60 vec_a;       // 0x40
    UnkDragVec_l0d1b60 prev_vec_a;  // 0x4c
    UnkDragVec_l0d1b60 vec_b;       // 0x58
    UnkDragVec_l0d1b60 prev_vec_b;  // 0x64
    u8 pad_0x70[0xc];
    f32 value;       // 0x7c
    f32 prev_value;  // 0x80
    u8 pad_0x84[0x75];
    u8 flag;       // 0xf9
    u8 prev_flag;  // 0xfa
    u8 pad_0xfb[0xd];
    u8 pad_0x108[0xf0];
    UnkDragSource_l0d1b60* source;  // 0x1f8
    f32 time_set;                   // 0x200
    f32 time_clear;                 // 0x204
};

struct UnkDragVelocity_l0d1b60 {
    f32 x, y, z;
};

void fn_71010d1b60(UnkDragMovement_l0d1b60* self, f32 dt) {
    self->prev_vec_a = self->vec_a;
    self->prev_vec_b = self->vec_b;
    self->prev_value = self->value;
    self->prev_flag = self->flag;
    f32 drag = self->source->drag;
    UnkDragVelocity_l0d1b60* vel = reinterpret_cast<UnkDragVelocity_l0d1b60*>(
        reinterpret_cast<u8*>(self) + 0x1d8);
    if (drag > 1.1920929e-7f && vel->x * vel->x + vel->y * vel->y + vel->z * vel->z > 1.1920929e-7f) {
        f32 s = drag * dt;
        f32 k = !(s >= 1.0f) ? s : 1.0f;
        vel->x = vel->x - k * vel->x;
        vel->y = vel->y - k * vel->y;
        vel->z = vel->z - k * vel->z;
        if (self->flag != 1 && vel->y > 0.001f) {
            self->flag = 1;
            self->vt->on_set(self);
        }
    }
    if (self->flag) {
        self->time_set = self->time_set + dt;
        self->time_clear = 0.0f;
    } else {
        self->time_clear = self->time_clear + dt;
        self->time_set = 0.0f;
    }
}
