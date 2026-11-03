// CFlockingSwarmControllerComponent: a per-frame step (fn_710073d870). Clears the word at +0x2a0, moves
// the value at +0x2bc toward the target (vtable slot 0x208) by at most the rate (slot 0x210) times the
// frame time, then runs slot 0x258 with the frame time.
#include "types.h"

struct UnkSwarmController;

struct UnkSwarmControllerVtable {
    u8 pad_0x0[0x208];
    f32 (*vf_0x208)(UnkSwarmController* self);
    f32 (*vf_0x210)(UnkSwarmController* self);
    u8 pad_0x218[0x40];
    void (*vf_0x258)(UnkSwarmController* self, f32 dt);
};

struct UnkSwarmController {
    UnkSwarmControllerVtable* vtable;
    u8 pad_0x8[0x298];
    u64 field_0x2a0;
    u8 pad_0x2a8[0x14];
    f32 field_0x2bc;
};

void fn_710073d870(UnkSwarmController* self, f32 dt) {
    self->field_0x2a0 = 0;
    f32 target = self->vtable->vf_0x208(self);
    f32 cur = self->field_0x2bc;
    f32 step = self->vtable->vf_0x210(self) * dt;
    f32 r;
    if (cur > target)
        r = target > cur - step ? target : cur - step;
    else
        r = target < cur + step ? target : cur + step;
    self->field_0x2bc = r;
    self->vtable->vf_0x258(self, dt);
}
