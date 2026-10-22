// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x90 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkWords {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkBlock32 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct __attribute__((packed)) UnkLongByte {
    u64 field_0x0;
    u8 field_0x8;
};

struct __attribute__((packed)) UnkLongWord {
    u64 field_0x0;
    u32 field_0x8;
};

struct UnkRecord {
    u8 pad_0x0[0x8];
    UnkBlock32 field_0x8;
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
    UnkLongByte field_0x58;
    u8 pad_0x61[7];
    UnkLongWord field_0x68;
    u8 pad_0x74[0xc];
    UnkWords field_0x80;
    UnkWeakHandle* field_0x90;
};

UnkRecord* fn_71004caf24(UnkRecord* self, const UnkRecord* other) {
    self->field_0x8 = other->field_0x8;
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
    self->field_0x58 = other->field_0x58;
    self->field_0x68 = other->field_0x68;
    self->field_0x80 = other->field_0x80;
    if (self != other)
        CopyWeakRef(&self->field_0x90, other->field_0x90);
    return self;
}
