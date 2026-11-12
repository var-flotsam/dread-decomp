// Unit 209 (a particle-style emitter, no reliable name): builds the emitter from its parameters. Names
// and layouts are placeholders.
#include <new>

#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEmitterOwner_b0253;

extern void* g_unk_7101a1dba8[];  // the emitter's vtable (GOT 0x7101cc6778)

struct UnkEmitterParams_b0253 {
    f32 start;       // 0x0
    f32 duration;    // 0x4
    f32 field_0x8;   // 0x8
    f32 field_0xc;   // 0xc
    f32 field_0x10;  // 0x10
    f32 field_0x14;  // 0x14
    f32 field_0x18;  // 0x18
    f32 field_0x1c;  // 0x1c
    UnkVec3f translation;  // 0x20
    UnkVec3f angle;        // 0x2c
    UnkVec3f field_0x38;   // 0x38
};
static_assert(sizeof(UnkEmitterParams_b0253) == 0x44, "UnkEmitterParams_b0253 layout");

struct UnkEmitter_b0253 {
    void** vtable;  // 0x0
    UnkEmitterOwner_b0253* owner;  // 0x8
    u8 pad_0x10[0x0];
    f32 time;      // 0x10
    f32 field_0x14;  // 0x14
    f32 field_0x18;  // 0x18
    UnkMatrix44 matrix;  // 0x1c
    UnkVec3f translation;  // 0x5c
    UnkVec3f field_0x68;   // 0x68
    f32 start;       // 0x74
    f32 duration;    // 0x78
    f32 field_0x7c;  // 0x7c
    f32 field_0x80;  // 0x80
    f32 field_0x84;  // 0x84
    f32 field_0x88;  // 0x88
    f32 field_0x8c;  // 0x8c
    f32 field_0x90;  // 0x90
    u8 active;       // 0x94
};
static_assert(__builtin_offsetof(UnkEmitter_b0253, active) == 0x94, "UnkEmitter_b0253 layout");

void fn_710016e03c(UnkEmitter_b0253* self, UnkEmitterOwner_b0253* owner, const UnkEmitterParams_b0253* params) {
    self->vtable = &g_unk_7101a1dba8[2];
    self->owner = owner;
    self->time = 0.0f;
    self->field_0x14 = params->duration;
    self->field_0x18 = 0.0f;
    new (&self->matrix) UnkMatrix44(fn_71000791a8(params->translation, params->angle, UnkVec3f(1.0f, 1.0f, 1.0f)));
    self->translation = params->translation;
    self->field_0x68 = params->field_0x38;
    self->start = params->start;
    self->duration = params->duration;
    self->field_0x7c = params->field_0x8;
    self->field_0x80 = params->field_0xc;
    self->field_0x84 = params->field_0x10;
    self->field_0x88 = params->field_0x14;
    self->field_0x8c = params->field_0x18;
    self->field_0x90 = params->field_0x1c;
    self->active = 0;
    if (params->start < 0.0f)
        self->active = 1;
    else
        self->active = self->time <= params->start + params->duration;
}
