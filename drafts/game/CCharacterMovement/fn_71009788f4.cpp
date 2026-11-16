// Unit 1445 (CCharacterMovement): initialises a movement state from a weight and a source's parameter
// block, keeping a second copy of the parameters, an identity transform and a valid flag.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMoveParams_l9788f4 {
    f32 values[23];

    UnkMoveParams_l9788f4& operator=(const UnkMoveParams_l9788f4& other) {
        for (u32 i = 0; i < 23; ++i)
            values[i] = other.values[i];
        return *this;
    }
};
static_assert(sizeof(UnkMoveParams_l9788f4) == 0x5c, "layout");

struct UnkVec3_l9788f4 {
    f32 x, y, z;
};

struct UnkMatrix44_l9788f4 {
    f32 m[4][4];

    void SetIdentity() {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }
};

struct UnkMoveSource_l9788f4 {
    u8 unk_0x0[0x38];
    UnkMoveParams_l9788f4 params;  // 0x38
};

struct UnkMoveState_l9788f4 {
    f32 weight;                      // 0x0
    UnkMoveParams_l9788f4 params;    // 0x4
    UnkVec3_l9788f4 offset;          // 0x60
    UnkVec3_l9788f4 velocity;        // 0x6c
    UnkMatrix44_l9788f4 transform;   // 0x78
    UnkMoveParams_l9788f4 initial;   // 0xb8
    u8 field_0x114;
    u8 field_0x115;
    u8 valid;                        // 0x116
};
static_assert(__builtin_offsetof(UnkMoveState_l9788f4, initial) == 0xb8, "layout");
static_assert(__builtin_offsetof(UnkMoveState_l9788f4, valid) == 0x116, "layout");

void fn_71009788f4(UnkMoveState_l9788f4* self, f32 weight, const UnkMoveSource_l9788f4* source) {
    self->weight = weight;
    self->params = source->params;
    self->offset.x = 0.0f;
    self->offset.y = 0.0f;
    self->offset.z = 0.0f;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    self->transform.SetIdentity();
    self->initial = source->params;
    self->valid = 1;
}
