// Copy assignment of a record that starts with a weak reference: unless assigned to itself, makes the
// reference refer to what the other one refers to, then copies the remaining fields.
#include "game_s2/UnkWeakHandle.h"

struct UnkRecord {
    UnkWeakHandle* field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
    u64 field_0x18;
};

UnkRecord* fn_710054715c(UnkRecord* self, const UnkRecord* other) {
    if (self != other)
        CopyWeakRef(&self->field_0x0, other->field_0x0);
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x14 = other->field_0x14;
    self->field_0x18 = other->field_0x18;
    return self;
}

// The same for a record in another unit with three words and a halfword after the reference
// (fn_7100ab805c).
struct UnkRecord_7100ab805c {
    UnkWeakHandle* field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u16 field_0x14;
};

UnkRecord_7100ab805c* fn_7100ab805c(UnkRecord_7100ab805c* self, const UnkRecord_7100ab805c* other) {
    if (self != other)
        CopyWeakRef(&self->field_0x0, other->field_0x0);
    self->field_0x8 = other->field_0x8;
    self->field_0xc = other->field_0xc;
    self->field_0x10 = other->field_0x10;
    self->field_0x14 = other->field_0x14;
    return self;
}
