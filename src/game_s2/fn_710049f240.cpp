// Copy assignment of a record: unless it is assigned to itself, copies its plain fields from `other`
// and makes the weak reference at +0x50 refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u8 field_0x28[12];
    u8 pad_0x34[0xc];
    u8 field_0x40[16];
    UnkWeakHandle* field_0x50;
    u8 field_0x58[12];
};

UnkRecord* fn_710049f240(UnkRecord* self, const UnkRecord* other) {
    if (self != other) {
        __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
        __builtin_memcpy(self->field_0x28, other->field_0x28, 12);
        __builtin_memcpy(self->field_0x40, other->field_0x40, 16);
        CopyWeakRef(&self->field_0x50, other->field_0x50);
        __builtin_memcpy(self->field_0x58, other->field_0x58, 12);
    }
    return self;
}
