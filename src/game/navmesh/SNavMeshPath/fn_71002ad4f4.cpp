// Copy assignment of a record: assigns its base part at +0x8 (fn_71002ae2f0), copies the plain fields
// at +0x1194c from `other`, then (unless it is assigned to itself) makes the weak reference at +0x11968
// refer to what the other one refers to.
#include "game_s2/UnkWeakHandle.h"

struct __attribute__((packed)) UnkTail {
    u8 field_0x0;
    u8 pad_0x1[3];
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
};

struct UnkRecord {
    u8 pad_0x0[0x8];
    u8 field_0x8[0x11944];
    UnkTail field_0x1194c;
    UnkWeakHandle* field_0x11968;
};

void fn_71002ae2f0(void* self, const void* other);

UnkRecord* fn_71002ad4f4(UnkRecord* self, const UnkRecord* other) {
    fn_71002ae2f0(self->field_0x8, other->field_0x8);
    self->field_0x1194c.field_0x0 = other->field_0x1194c.field_0x0;
    self->field_0x1194c.field_0x4 = other->field_0x1194c.field_0x4;
    self->field_0x1194c.field_0x8 = other->field_0x1194c.field_0x8;
    self->field_0x1194c.field_0xc = other->field_0x1194c.field_0xc;
    self->field_0x1194c.field_0x10 = other->field_0x1194c.field_0x10;
    self->field_0x1194c.field_0x14 = other->field_0x1194c.field_0x14;
    self->field_0x1194c.field_0x18 = other->field_0x1194c.field_0x18;
    if (self != other)
        CopyWeakRef(&self->field_0x11968, other->field_0x11968);
    return self;
}
