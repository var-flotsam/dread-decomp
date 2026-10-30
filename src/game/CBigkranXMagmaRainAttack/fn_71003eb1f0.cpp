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

// The same for a record in another unit, with an 8-byte field at +0x28 and the weak reference at
// +0x48 (fn_71005c5b94).
struct UnkRecord_71005c5b94 {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[8];
    u8 pad_0x30[0x8];
    u8 field_0x38[16];
    UnkWeakHandle* field_0x48;
    u64 field_0x50;
};

UnkRecord_71005c5b94* fn_71005c5b94(UnkRecord_71005c5b94* self, const UnkRecord_71005c5b94* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 8);
    __builtin_memcpy(self->field_0x38, other->field_0x38, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x48, other->field_0x48);
    self->field_0x50 = other->field_0x50;
    return self;
}
