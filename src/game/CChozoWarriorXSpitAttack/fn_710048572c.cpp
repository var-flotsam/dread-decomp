// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0xc0 refer to what the other one refers to.
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
    u8 field_0x48[32];
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u8 field_0x98[16];
    u8 pad_0xa8[0x8];
    u8 field_0xb0[16];
    UnkWeakHandle* field_0xc0;
};

UnkRecord* fn_710048572c(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->field_0x40 = other->field_0x40;
    __builtin_memcpy(self->field_0x48, other->field_0x48, 32);
    self->field_0x68 = other->field_0x68;
    self->field_0x6c = other->field_0x6c;
    self->field_0x70 = other->field_0x70;
    self->field_0x74 = other->field_0x74;
    self->field_0x78 = other->field_0x78;
    self->field_0x7c = other->field_0x7c;
    self->field_0x80 = other->field_0x80;
    self->field_0x84 = other->field_0x84;
    self->field_0x88 = other->field_0x88;
    self->field_0x8c = other->field_0x8c;
    self->field_0x90 = other->field_0x90;
    self->field_0x94 = other->field_0x94;
    __builtin_memcpy(self->field_0x98, other->field_0x98, 16);
    __builtin_memcpy(self->field_0xb0, other->field_0xb0, 16);
    if (self != other)
        CopyWeakRef(&self->field_0xc0, other->field_0xc0);

    return self;
}
