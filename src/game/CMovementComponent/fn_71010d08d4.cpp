// A setter in the CMovementComponent unit (unit 2688 of tools/tu_map.py): stores a vector at +0x1d8
// and, when its y is above 0.001 and `start` is set, raises the flag at +0xf9 (once) through the
// virtual at 0x260. Names and layouts are placeholders.
#include "types.h"

struct UnkMovement_71010d08d4;

struct UnkMovementVtbl_71010d08d4 {
    u8 pad_0x0[0x260];
    void (*vf_0x260)(UnkMovement_71010d08d4* self);
};

struct UnkMovementVec_71010d08d4 {
    f32 x, y, z;
};

struct UnkMovement_71010d08d4 {
    UnkMovementVtbl_71010d08d4* vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0xde];
    UnkMovementVec_71010d08d4 field_0x1d8;
};
static_assert(__builtin_offsetof(UnkMovement_71010d08d4, field_0x1d8) == 0x1d8, "UnkMovement_71010d08d4 layout");

void fn_71010d08d4(UnkMovement_71010d08d4* self, const UnkMovementVec_71010d08d4* v, bool start) {
    self->field_0x1d8.x = v->x;
    self->field_0x1d8.y = v->y;
    self->field_0x1d8.z = v->z;
    if (self->field_0x1d8.y > 0.001f && start && self->field_0xf9 != true) {
        self->field_0xf9 = true;
        self->vtable->vf_0x260(self);
    }
}
