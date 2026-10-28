// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x50 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[16];
    u8 pad_0x38[0x8];
    u8 field_0x40[16];
    UnkWeakHandle* field_0x50;
    u64 field_0x58;
};

UnkRecord* fn_71003eb1f0(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 16);
    __builtin_memcpy(self->field_0x40, other->field_0x40, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x50, other->field_0x50);
    self->field_0x58 = other->field_0x58;
    return self;
}
