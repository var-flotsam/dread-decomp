// Sets an enabled flag (0xf9) on an object. When it changes: tells the owner (if any) about the
// new state, refreshes a block of values from the object's description, notifies itself through
// a virtual hook, and mirrors the flag into the description.
#include "types.h"

struct UnkStruct_710020c8ac;

struct UnkOwner_710020c8ac;
void fn_71001eef58(UnkOwner_710020c8ac* owner, UnkStruct_710020c8ac* obj, bool value);
void fn_71001eef74(UnkOwner_710020c8ac* owner, UnkStruct_710020c8ac* obj, bool value);

struct UnkDesc_710020c8ac {
    u8 unk_0x0[0x28];
    u32 field_0x28;
    u8 unk_0x2c[0xc];
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    u8 unk_0x6e[0x72];
    bool field_0xe0;
};

struct UnkValues_710020c8ac {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    bool field_0x34;
    bool field_0x35;
};

struct UnkVtbl_710020c8ac {
    void* pad_0x0[6];
    void (*vf_0x30)(UnkStruct_710020c8ac* self, UnkValues_710020c8ac* values);
};

struct UnkStruct_710020c8ac {
    UnkVtbl_710020c8ac* vtbl;
    UnkOwner_710020c8ac* owner;
    u8 unk_0x10[0x78];
    u32 field_0x88;
    u8 unk_0x8c[0xc];
    UnkValues_710020c8ac values;  // 0x98
    u8 unk_0xd0[0x26];
    bool field_0xf6;
    u8 unk_0xf7[2];
    bool enabled;  // 0xf9
    u8 unk_0xfa[0xe];
    UnkDesc_710020c8ac* desc;
};

void fn_710020c8ac(UnkStruct_710020c8ac* self, bool value) {
    if (value == self->enabled)
        return;
    self->enabled = value;
    if (self->owner) {
        fn_71001eef58(self->owner, self, value);
        fn_71001eef74(self->owner, self, self->enabled && self->field_0xf6);
    }
    UnkDesc_710020c8ac* desc = self->desc;
    self->field_0x88 = desc->field_0x28;
    self->values.field_0x0 = desc->field_0x38;
    self->values.field_0x4 = desc->field_0x3c;
    self->values.field_0x8 = desc->field_0x40;
    self->values.field_0xc = desc->field_0x44;
    self->values.field_0x10 = desc->field_0x48;
    self->values.field_0x14 = desc->field_0x4c;
    self->values.field_0x18 = desc->field_0x50;
    self->values.field_0x20 = desc->field_0x58;
    self->values.field_0x28 = desc->field_0x60;
    self->values.field_0x2c = desc->field_0x64;
    self->values.field_0x30 = desc->field_0x68;
    self->values.field_0x34 = desc->field_0x6c;
    self->values.field_0x35 = desc->field_0x6d;
    self->vtbl->vf_0x30(self, &self->values);
    UnkDesc_710020c8ac* d = self->desc;
    d->field_0xe0 = self->enabled;
}
