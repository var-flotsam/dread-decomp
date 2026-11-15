// nn::atk unit: constructor of an object with a base part, an intrusive list at +0xe0 and a sub-object at +0x138.
// Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101cb86b8[];

struct UnkAtkListNode {
    UnkAtkListNode* prev;
    UnkAtkListNode* next;
};

struct UnkAtkObj {
    void* vtable;
    u8 pad_0x08[0xb8];
    void* field_0xc0;
    void* field_0xc8;
    void* field_0xd0;
    void* field_0xd8;
    UnkAtkListNode list;
    u8 pad_0xf0[0x138 - 0xf0];
    u8 field_0x138[0x14c - 0x138];
    u32 field_0x14c;
    u8 field_0x150;
    u8 pad_0x151[0x16d - 0x151];
    u8 field_0x16d;
    u8 field_0x16e;
    u8 pad_0x16f[0x170 - 0x16f];
    u64 field_0x170;
    u64 field_0x178;
    u64 field_0x180;
    u64 field_0x188;
    u64 field_0x190;
    s32 field_0x198;
    u8 pad_0x19c[4];
    u64 field_0x1a0;
    u32 field_0x1a8;
};

void fn_710120e83c(UnkAtkObj* self);
void fn_710122aa9c(void* sub);

void fn_7101230b58(UnkAtkObj* self) {
    fn_710120e83c(self);
    self->list.prev = &self->list;
    self->list.next = &self->list;
    self->field_0xc0 = g_unk_7101cb86b8 + 0x80;
    self->field_0xc8 = &self->field_0xc8;
    self->vtable = g_unk_7101cb86b8 + 0x10;
    self->field_0xd0 = &self->field_0xc8;
    self->field_0xd8 = g_unk_7101cb86b8 + 0xa8;
    fn_710122aa9c(self->field_0x138);
    self->field_0x150 = 0;
    self->field_0x14c = 0;
    self->field_0x16d = 0;
    self->field_0x16e = 0;
    self->field_0x190 = 0;
    self->field_0x198 = -1;
    self->field_0x1a0 = 0;
    self->field_0x1a8 = 0;
    self->field_0x170 = 0;
    self->field_0x178 = 0;
    self->field_0x180 = 0;
    self->field_0x188 = 0;
}
