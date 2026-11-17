// Unit 1445 (CCharacterMovement): sets up a movement state from a scale and a parameter block, kept twice
// (current and initial), with a cleared offset pair and an identity transform. Names and layouts are
// placeholders.
#include "types.h"

struct UnkMovementParams_l978740 {
    u32 values[23];
};

struct UnkMovementState_l978740 {
    f32 scale;                          // 0x0
    UnkMovementParams_l978740 current;  // 0x4
    f32 offset_a[3];                    // 0x60
    f32 offset_b[3];                    // 0x6c
    f32 transform[16];                  // 0x78
    UnkMovementParams_l978740 initial;  // 0xb8
    u8 field_0x114;
    u8 field_0x115;
    u8 field_0x116;
    u8 pad_0x117;
};
static_assert(__builtin_offsetof(UnkMovementState_l978740, transform) == 0x78, "layout");
static_assert(__builtin_offsetof(UnkMovementState_l978740, field_0x116) == 0x116, "layout");

static inline void CopyParams(UnkMovementParams_l978740* dst, const UnkMovementParams_l978740* src) {
    for (u32 i = 0; i < 23; ++i)
        dst->values[i] = src->values[i];
}

void fn_7100978740(UnkMovementState_l978740* self, f32 scale, const UnkMovementParams_l978740* params) {
    self->scale = scale;
    CopyParams(&self->current, params);
    self->offset_a[0] = 0.0f;
    self->offset_a[1] = 0.0f;
    self->offset_a[2] = 0.0f;
    self->offset_b[0] = 0.0f;
    self->offset_b[1] = 0.0f;
    self->offset_b[2] = 0.0f;
    for (u32 i = 0; i < 16; ++i)
        self->transform[i] = 0.0f;
    self->transform[15] = 1.0f;
    self->transform[10] = 1.0f;
    self->transform[5] = 1.0f;
    self->transform[0] = 1.0f;
    CopyParams(&self->initial, params);
    self->field_0x116 = 1;
}
