// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0xa0 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[32];
    u64 field_0x48;
    u8 field_0x50[24];
    u8 field_0x68[28];
    u8 pad_0x84[0xc];
    u8 field_0x90[16];
    UnkWeakHandle* field_0xa0;
    u64 field_0xa8;
};

UnkRecord* fn_710042a63c(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 32);
    self->field_0x48 = other->field_0x48;
    __builtin_memcpy(self->field_0x50, other->field_0x50, 24);
    __builtin_memcpy(self->field_0x68, other->field_0x68, 28);
    __builtin_memcpy(self->field_0x90, other->field_0x90, 16);
    if (self != other)
        CopyWeakRef(&self->field_0xa0, other->field_0xa0);
    self->field_0xa8 = other->field_0xa8;
    return self;
}
