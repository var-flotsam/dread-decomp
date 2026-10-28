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
    u8 field_0x60[1];
};

UnkRecord* fn_710043b0f4(UnkRecord* self, const UnkRecord* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    __builtin_memcpy(self->field_0x28, other->field_0x28, 32);
    self->field_0x48 = other->field_0x48;
    self->field_0x50 = other->field_0x50;
    if (self != other)
        CopyWeakRef(&self->field_0x58, other->field_0x58);
    __builtin_memcpy(self->field_0x60, other->field_0x60, 1);
    return self;
}
