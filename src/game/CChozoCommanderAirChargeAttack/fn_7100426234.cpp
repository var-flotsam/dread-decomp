// Copy assignment of a record: copies its plain fields from `other`, then (unless it is assigned to
// itself) makes the weak reference at +0x80 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[32];
    u64 field_0x48;
    u8 field_0x50[12];
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u8 pad_0x68[0x8];
    u8 field_0x70[16];
    UnkWeakHandle* field_0x80;
};

UnkRecord* fn_7100426234(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 32);
    self->field_0x48 = other->field_0x48;
    __builtin_memcpy(self->field_0x50, other->field_0x50, 12);
    self->field_0x5c = other->field_0x5c;
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
    __builtin_memcpy(self->field_0x70, other->field_0x70, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x80, other->field_0x80);
    return self;
}
