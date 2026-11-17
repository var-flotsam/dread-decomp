// Unit 1445 (CCharacterMovement): initialises a movement state from a weight and a source's parameter
// block, keeping a second copy of the parameters, an identity transform and a valid flag.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
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

struct UnkMoveSource_l9788f4 {
    u8 unk_0x0[0x38];
    UnkMoveParams_l9788f4 params;  // 0x38
};

struct UnkMoveState_l9788f4 {
    f32 weight;                      // 0x0
    UnkMoveParams_l9788f4 params;    // 0x4
    base::math::CVector3D offset;          // 0x60
    base::math::CVector3D velocity;        // 0x6c
    UnkMatrix44 transform;   // 0x78
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
    self->transform = UnkMatrix44{};
    self->offset.x = 0.0f;
    self->offset.y = 0.0f;
    self->offset.z = 0.0f;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    self->transform.m[15] = 1.0f;
    self->transform.m[10] = 1.0f;
    self->transform.m[5] = 1.0f;
    self->transform.m[0] = 1.0f;
    self->initial = source->params;
    self->valid = 1;
}
