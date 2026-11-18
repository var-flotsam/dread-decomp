// CCharacterMovement (unit 1445): resets a movement state from a set of parameters: the start value,
// the parameters (kept twice, as given and as current), a zeroed offset and an identity transform.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "types.h"

// 23 words of movement parameters, copied word by word.
struct UnkMoveParams_l978aa8 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;

    UnkMoveParams_l978aa8& operator=(const UnkMoveParams_l978aa8& other) {
        if (&other != this) {
            field_0x0 = other.field_0x0;
            field_0x4 = other.field_0x4;
            field_0x8 = other.field_0x8;
            field_0xc = other.field_0xc;
            field_0x10 = other.field_0x10;
            field_0x14 = other.field_0x14;
            field_0x18 = other.field_0x18;
            field_0x1c = other.field_0x1c;
            field_0x20 = other.field_0x20;
            field_0x24 = other.field_0x24;
            field_0x28 = other.field_0x28;
            field_0x2c = other.field_0x2c;
            field_0x30 = other.field_0x30;
            field_0x34 = other.field_0x34;
            field_0x38 = other.field_0x38;
            field_0x3c = other.field_0x3c;
            field_0x40 = other.field_0x40;
            field_0x44 = other.field_0x44;
            field_0x48 = other.field_0x48;
            field_0x4c = other.field_0x4c;
            field_0x50 = other.field_0x50;
            field_0x54 = other.field_0x54;
            field_0x58 = other.field_0x58;
        }
        return *this;
    }
};
static_assert(sizeof(UnkMoveParams_l978aa8) == 0x5c, "UnkMoveParams_l978aa8 layout");

struct UnkMoveSource_l978aa8 {
    u8 pad_0x0[0x38];
    UnkMoveParams_l978aa8 field_0x38;
};

struct UnkMoveState_l978aa8 {
    f32 field_0x0;
    UnkMoveParams_l978aa8 field_0x4;
    base::math::CVector3D field_0x60;
    base::math::CVector3D field_0x6c;
    UnkMatrix44 field_0x78;
    UnkMoveParams_l978aa8 field_0xb8;
    bool field_0x114;
    bool field_0x115;
    bool field_0x116;
};
static_assert(__builtin_offsetof(UnkMoveState_l978aa8, field_0x114) == 0x114, "UnkMoveState_l978aa8 layout");

void fn_7100978aa8(UnkMoveState_l978aa8* self, const UnkMoveSource_l978aa8* source, f32 value) {
    self->field_0x0 = value;
    self->field_0x4 = source->field_0x38;
    self->field_0x6c.z = 0.0f;
    self->field_0x6c.y = 0.0f;
    self->field_0x6c.x = 0.0f;
    self->field_0x60.z = 0.0f;
    self->field_0x60.y = 0.0f;
    self->field_0x60.x = 0.0f;
    self->field_0x78 = UnkMatrix44{};
    self->field_0x78.m[15] = 1.0f;
    self->field_0x78.m[10] = 1.0f;
    self->field_0x78.m[5] = 1.0f;
    self->field_0x78.m[0] = 1.0f;
    self->field_0xb8 = self->field_0x4;
    self->field_0x114 = false;
    self->field_0x115 = false;
    self->field_0x116 = true;
}
