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
