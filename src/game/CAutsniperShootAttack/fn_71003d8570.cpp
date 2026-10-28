// Copy assignment of a record: copies its plain fields from `other`, makes the weak reference at +0x48
// refer to what the other one refers to (unless it is assigned to itself), copies the rest, assigns the
// sub-record at +0xd0 (fn_71002ad420) and copies the flag byte at +0x12b58.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u64 field_0x28;
    u8 pad_0x30[0x8];
    u8 field_0x38[16];
    UnkWeakHandle* field_0x48;
    u64 field_0x50;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u64 field_0x88;
    u8 field_0x90[32];
    u8 field_0xb0[29];
    u8 pad_0xcd[0x3];
    u8 field_0xd0[0x12a88];
    u8 field_0x12b58;
};

void fn_71002ad420(void* self, const void* other);

UnkRecord* fn_71003d8570(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    self->field_0x28 = other->field_0x28;
    __builtin_memcpy(self->field_0x38, other->field_0x38, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x48, other->field_0x48);
    self->field_0x50 = other->field_0x50;
    self->field_0x58 = other->field_0x58;
    self->field_0x5c = other->field_0x5c;
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
    self->field_0x68 = other->field_0x68;
    self->field_0x6c = other->field_0x6c;
    self->field_0x70 = other->field_0x70;
    self->field_0x74 = other->field_0x74;
    self->field_0x78 = other->field_0x78;
    self->field_0x7c = other->field_0x7c;
    self->field_0x80 = other->field_0x80;
    self->field_0x84 = other->field_0x84;
    self->field_0x88 = other->field_0x88;
    __builtin_memcpy(self->field_0x90, other->field_0x90, 32);
    __builtin_memcpy(self->field_0xb0, other->field_0xb0, 29);
    fn_71002ad420(self->field_0xd0, other->field_0xd0);
    self->field_0x12b58 = other->field_0x12b58;
    return self;
}
