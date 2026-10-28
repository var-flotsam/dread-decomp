// Copy assignment of a record: copies its plain fields from `other`, makes the weak reference at +0x88
// refer to what the other one refers to (unless it is assigned to itself), copies more fields, assigns
// the sub-records at +0xa0 (fn_7100d62ca8) and +0x1d8 (fn_71004c5c2c) and copies 11 bytes at +0x77c.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u64 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u8 field_0x58[9];
    u8 pad_0x61[7];
    u64 field_0x68;
    u8 pad_0x70[8];
    u8 field_0x78[16];
    u8 pad_0x88[0];
    UnkWeakHandle* field_0x88;
    u8 field_0x90[12];
    u8 pad_0x9c[4];
    u8 field_0xa0[0x120];
    u8 field_0x1c0[24];
    u8 field_0x1d8[0x5a4];
    u8 field_0x77c[11];
};

void fn_7100d62ca8(void* self, const void* other);
void fn_71004c5c2c(void* self, const void* other);

// NON_MATCHING(minor): the 24-byte copy at +0x1c0 and the 11-byte copy at +0x77c load and store in a different order
UnkRecord* fn_71004c5a14(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    self->field_0x40 = other->field_0x40;
    self->field_0x44 = other->field_0x44;
    self->field_0x48 = other->field_0x48;
    self->field_0x4c = other->field_0x4c;
    self->field_0x54 = other->field_0x54;
    self->field_0x50 = other->field_0x50;
    __builtin_memcpy(self->field_0x58, other->field_0x58, 9);
    self->field_0x68 = other->field_0x68;
    __builtin_memcpy(self->field_0x78, other->field_0x78, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x88, other->field_0x88);
    __builtin_memcpy(self->field_0x90, other->field_0x90, 12);
    fn_7100d62ca8(self->field_0xa0, other->field_0xa0);
    __builtin_memcpy(self->field_0x1c0, other->field_0x1c0, 24);
    fn_71004c5c2c(self->field_0x1d8, other->field_0x1d8);
    __builtin_memcpy(self->field_0x77c, other->field_0x77c, 11);
    return self;
}
