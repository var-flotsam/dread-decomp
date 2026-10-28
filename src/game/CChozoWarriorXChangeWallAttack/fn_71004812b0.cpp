// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x60 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u64 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u8 pad_0x3c[0x4];
    u64 field_0x40;
    u8 pad_0x48[0x8];
    u8 field_0x50[16];
    UnkWeakHandle* field_0x60;
};

UnkRecord* fn_71004812b0(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->field_0x40 = other->field_0x40;
    __builtin_memcpy(self->field_0x50, other->field_0x50, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x60, other->field_0x60);
    return self;
}
