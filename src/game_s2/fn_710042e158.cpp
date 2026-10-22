// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x90 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[32];
    u64 field_0x48;
    u64 field_0x50;
    u8 field_0x58[28];
    u8 pad_0x74[0xc];
    u8 field_0x80[16];
    UnkWeakHandle* field_0x90;
};

UnkRecord* fn_710042e158(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 32);
    self->field_0x48 = other->field_0x48;
    self->field_0x50 = other->field_0x50;
    __builtin_memcpy(self->field_0x58, other->field_0x58, 28);
    __builtin_memcpy(self->field_0x80, other->field_0x80, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x90, other->field_0x90);
    return self;
}
