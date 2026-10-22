// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x60 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u64 field_0x28;
    u8 field_0x30[29];
    u8 pad_0x4d[0x3];
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    UnkWeakHandle* field_0x60;
    u64 field_0x68;
};

UnkRecord* fn_7100467b2c(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x30, other->field_0x30, 29);
    self->field_0x28 = other->field_0x28;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x58 = other->field_0x58;
    self->field_0x5c = other->field_0x5c;
    if (self != other)
        CopyWeakRef(&self->field_0x60, other->field_0x60);
    self->field_0x68 = other->field_0x68;
    return self;
}
