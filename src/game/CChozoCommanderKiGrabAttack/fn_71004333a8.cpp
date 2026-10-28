// Resets an attack state's cached values: looks up the owner's body and its field at +0x2b0 (or
// null) for a query, runs the owner's attack lookup (with a fixed key) when a flag is set, sets four
// floats to -1 and continues into the state's next step with the limit read from a member object
// (or INT_MAX when there is none; the step takes a float only when there is one).
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkLimit {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkStep {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x88];
    UnkLimit* field_0x98;
    u8 pad_0xa0[0x9c];
    u8 field_0x13c;
    u8 pad_0x13d[3];
    f32 field_0x140;
    f32 field_0x144;
    f32 field_0x148;
    f32 field_0x14c;
};

void fn_7100352b14(void* body_field, UnkActor* owner, u32 flag);
void* fn_71004321a0(UnkStep* self, u32 limit);
void* fn_71004321a0(UnkStep* self, u32 limit, f32 value);

// NON_MATCHING(minor): in the float-argument tail call the -1.0f is materialised after the frame
// pop instead of before it (scheduling only)
void* fn_71004333a8(UnkStep* self) {
    UnkActor* body = fn_7100d7c990(self->field_0x8);
    fn_7100352b14(body ? body->field_0x2b0 : nullptr, self->field_0x8, 1);
    if (self->field_0x13c) {
        fn_7101177ea0(static_cast<UnkAnimationComponent*>(self->field_0x8->field_0x220), 0xf4061ff25a2163d6ull);
        self->field_0x140 = 0.0f;
        self->field_0x144 = -1.0f;
        self->field_0x148 = -1.0f;
        self->field_0x14c = -1.0f;
        u32 limit = self->field_0x98 ? self->field_0x98->field_0x10 : 0x7fffffff;
        return fn_71004321a0(self, limit);
    }
    self->field_0x140 = 0.0f;
    self->field_0x144 = -1.0f;
    self->field_0x148 = -1.0f;
    self->field_0x14c = -1.0f;
    u32 limit = self->field_0x98 ? self->field_0x98->field_0x10 : 0x7fffffff;
    return fn_71004321a0(self, limit, -1.0f);
}
