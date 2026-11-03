#include "types.h"

struct UnkUnit301Tail {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkUnit301Value {
    u8 unk_0x0[0x8];
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    UnkUnit301Tail tail_0x20;
    u32 field_0x38;
};

UnkUnit301Value* fn_7100212530(UnkUnit301Value* self, const UnkUnit301Value* other, u32 tag) {
    self->field_0x8 = other->field_0x8;
    self->field_0xc = other->field_0xc;
    self->field_0x10 = other->field_0x10;
    self->field_0x14 = other->field_0x14;
    self->field_0x18 = other->field_0x18;
    self->field_0x1c = other->field_0x1c;
    self->tail_0x20 = other->tail_0x20;
    self->field_0x38 = tag | 0x80000000u;
    return self;
}
