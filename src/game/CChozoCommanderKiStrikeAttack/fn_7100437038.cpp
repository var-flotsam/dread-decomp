// Copy assignments of a plain 0x88-byte record (two identical functions).
#include "types.h"

struct UnkRecord {
    u8 unk_0x0[8];
    u64 field_0x8[2];
    u64 field_0x18[2];
    u64 field_0x28[2];
    u64 field_0x38[2];
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u8 field_0x84;
};

__attribute__((always_inline)) static inline void Assign(UnkRecord* self, const UnkRecord* other) {
    if (self == other)
        return;
    u64 a0 = other->field_0x8[0], a1 = other->field_0x8[1];
    u64 b0 = other->field_0x18[0], b1 = other->field_0x18[1];
    self->field_0x18[0] = b0;
    self->field_0x18[1] = b1;
    self->field_0x8[0] = a0;
    self->field_0x8[1] = a1;
    u64 c0 = other->field_0x28[0], c1 = other->field_0x28[1];
    u64 d0 = other->field_0x38[0], d1 = other->field_0x38[1];
    self->field_0x38[0] = d0;
    self->field_0x38[1] = d1;
    self->field_0x28[0] = c0;
    self->field_0x28[1] = c1;
    self->field_0x48 = other->field_0x48;
    u64 e0 = other->field_0x50, e1 = other->field_0x58, e2 = other->field_0x60;
    u32 e3 = other->field_0x68;
    self->field_0x50 = e0;
    self->field_0x68 = e3;
    self->field_0x58 = e1;
    self->field_0x60 = e2;
    self->field_0x6c = other->field_0x6c;
    self->field_0x70 = other->field_0x70;
    self->field_0x74 = other->field_0x74;
    self->field_0x78 = other->field_0x78;
    self->field_0x7c = other->field_0x7c;
    self->field_0x80 = other->field_0x80;
    self->field_0x84 = other->field_0x84;
}

// NON_MATCHING(minor): the stores of 0x50 and 0x58 pair differently (stp 0x50/0x58 + str 0x60 here, stp 0x58/0x60 + str 0x50 in the original).
void fn_7100437038(UnkRecord* self, const UnkRecord* other) { Assign(self, other); }
// NON_MATCHING(minor): as fn_7100437038.
void fn_71004370bc(UnkRecord* self, const UnkRecord* other) { Assign(self, other); }
