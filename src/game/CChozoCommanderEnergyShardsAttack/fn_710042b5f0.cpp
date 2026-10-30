// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x58 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[32];
    u64 field_0x48;
    u64 field_0x50;
    UnkWeakHandle* field_0x58;
};

UnkRecord* fn_710042b5f0(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 32);
    self->field_0x48 = other->field_0x48;
    self->field_0x50 = other->field_0x50;
    if (self != other)
        CopyWeakRef(&self->field_0x58, other->field_0x58);
    return self;
}

// The same for a record in another unit, with the weak reference at +0x60 (fn_71005f3ce8).
struct UnkRecord_71005f3ce8 {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[8];
    u8 field_0x30[21];
    u8 pad_0x45[0xb];
    u8 field_0x50[16];
    UnkWeakHandle* field_0x60;
};

UnkRecord_71005f3ce8* fn_71005f3ce8(UnkRecord_71005f3ce8* self, const UnkRecord_71005f3ce8* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 8);
    __builtin_memcpy(self->field_0x30, other->field_0x30, 21);
    __builtin_memcpy(self->field_0x50, other->field_0x50, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x60, other->field_0x60);
    return self;
}
