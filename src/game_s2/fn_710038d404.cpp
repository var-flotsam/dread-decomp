// Constructor of an object derived from fn_7100393e68's: sets its vtable, clears its members, names
// six string ids with the empty string, sets some flags, gives a second vtable-bearing sub-object
// at +0xa8 and finishes in vtable slot 0x18 of the object.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

typedef u64 u64_a4 __attribute__((aligned(4)));

typedef void** __attribute__((may_alias)) UnkVtable;

struct UnkNode {
    UnkVtable vtable;
    u8 pad_0x8[0x28];
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u8 field_0x50;
    u8 pad_0x51[3];
    u32 field_0x54;
    u64 field_0x58;
    u64 field_0x60;
    UnkStrId field_0x68;
    UnkStrId field_0x70;
    UnkStrId field_0x78;
    UnkStrId field_0x80;
    UnkStrId field_0x88;
    UnkStrId field_0x90;
    u16 field_0x98;
    u8 field_0x9a;
    u8 pad_0x9b;
    u64_a4 field_0x9c;
    u16 field_0xa4;
    u8 pad_0xa6[2];
    void** field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    u64 field_0xc0;
    u8 pad_0xc8;
    u8 field_0xc9;

    void vf_0x18() {
        reinterpret_cast<void (*)(UnkNode*)>(vtable[3])(this);
    }
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a26730[];
extern void* g_unk_7101a22d48[];

void fn_710038d404(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a26730 + 2;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0x7fffffff;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    new (&self->field_0x68) UnkStrId("", true);
    new (&self->field_0x70) UnkStrId("", true);
    new (&self->field_0x78) UnkStrId("", true);
    new (&self->field_0x80) UnkStrId("", true);
    new (&self->field_0x88) UnkStrId("", true);
    new (&self->field_0x90) UnkStrId("", true);
    self->field_0x98 = 0;
    self->field_0x9a = 0;
    self->field_0x9c = 0;
    self->field_0xa4 = 0;
    self->field_0xa8 = g_unk_7101a22d48 + 2;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 0;
    self->field_0xc9 = 0;
    self->vf_0x18();
}
