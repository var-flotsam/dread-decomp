// Copy assignment of a 0x68-byte struct: trivially copyable blocks around one non-trivial
// member at +0x28.
#include "types.h"

struct UnkStruct_0x28 {
    u8 unk_0x0[0x28];
};

void fn_71000ce19c(UnkStruct_0x28* self, const UnkStruct_0x28* other);

struct UnkBlock_0x20 {
    u64 field_0x0, field_0x8, field_0x10, field_0x18;
};

struct UnkBlock_0x10 {
    u64 field_0x0, field_0x8;
};

struct UnkStruct_0x68 {
    u8 unk_0x0[0x8];
    UnkBlock_0x20 field_0x8;
    UnkStruct_0x28 field_0x28;
    UnkBlock_0x10 field_0x50;
    u64 field_0x60;
};

UnkStruct_0x68* fn_71005b7568(UnkStruct_0x68* self, const UnkStruct_0x68* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        fn_71000ce19c(&self->field_0x28, &other->field_0x28);
        self->field_0x50 = other->field_0x50;
        self->field_0x60 = other->field_0x60;
    }
    return self;
}
