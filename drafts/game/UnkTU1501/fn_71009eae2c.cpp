// CSamusMovement's unit (unit 1501): tracks a stick flick (the stick x past -0.7071 with y at 1.0, then back past
// +0.7071) in the movement's input state at +0x5190; a flick counts once the stick has been through both ends.
// Names and layouts are placeholders.
#include "types.h"

struct UnkObjectState_l9eae2c {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkObject_l9eae2c {
    u8 pad_0x0[0x50];
    UnkObjectState_l9eae2c* state;  // 0x50
};

struct UnkActor_l9eae2c {
    u8 pad_0x0[0x2b0];
    UnkObject_l9eae2c* field_0x2b0;  // 0x2b0
};

struct UnkFlick_l9eae2c {
    u32 count;      // 0x0
    u32 completed;  // 0x4
    bool armed;     // 0x8
    u8 pad_0x9[3];
    u32 field_0xc;
    u32 field_0x10;
    u16 field_0x14;
};

struct UnkMovement_l9eae2c {
    u8 pad_0x0[0x30];
    UnkActor_l9eae2c* actor;  // 0x30
    u8 pad_0x38[0x6b4];
    f32 stick_x;  // 0x6ec
    f32 stick_y;  // 0x6f0
    u8 pad_0x6f4[0x4a9c];
    UnkFlick_l9eae2c flick;  // 0x5190
};
static_assert(__builtin_offsetof(UnkMovement_l9eae2c, stick_x) == 0x6ec, "layout");
static_assert(__builtin_offsetof(UnkMovement_l9eae2c, flick) == 0x5190, "layout");

static inline bool IsNearOne(f32 value) {
    const f32 epsilon = 0.001f;
    return value + epsilon >= 1.0f && value - epsilon <= 1.0f;
}

void fn_71009eae2c(UnkMovement_l9eae2c* self) {
    UnkFlick_l9eae2c* flick = &self->flick;
    UnkObject_l9eae2c* object = self->actor->field_0x2b0;
    if (!(object && object->state && (object->state->kind & ~1u) == 2)) {
        bool armed = flick->armed;
        flick->completed = 0;
        if (armed) {
            if (self->stick_x < -0.70710678f && IsNearOne(self->stick_y)) {
                flick->armed = false;
                flick->field_0x10 = 0;
                u32 count = flick->count++;
                if (count >= 1) {
                    flick->completed = 1;
                    flick->field_0x14 = 1;
                }
                return;
            }
        } else if (!(self->stick_y > 0.5f)) {
            flick->armed = true;
            return;
        }
        if (flick->count != 1)
            return;
        if (!(self->stick_x > 0.70710678f && IsNearOne(self->stick_y)))
            return;
        flick->armed = false;
        flick->count = 0;
        flick->completed = 0;
        flick->field_0x10 = flick->field_0xc;
        return;
    }
    flick->armed = false;
    flick->count = 0;
    flick->completed = 0;
    flick->field_0x10 = flick->field_0xc;
}
