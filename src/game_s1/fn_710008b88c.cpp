// Constructor of a 0xb0-byte state object: three allocator handles, counters and flags set to
// their defaults, and an embedded sub-object at +0x58.
#include "types.h"

struct UnkAllocator;
UnkAllocator* fn_7100083990();
void fn_710008377c(void* self);
extern void* g_unk_7101a1a4c8[];

struct UnkState_0xb0 {
    UnkAllocator* alloc_0x0;
    s32 field_0x8;
    s32 field_0xc, field_0x10, field_0x14, field_0x18, field_0x1c;
    s32 field_0x20;
    u8 field_0x24;
    u8 pad_0x25[3];
    s32 field_0x28;
    u8 pad_0x2c[4];
    UnkAllocator* alloc_0x30;
    void* field_0x38;
    void* field_0x40;
    void* vtable_0x48;
    void* field_0x50;
    u8 sub_0x58[0x28];
    s32 field_0x80;
    u8 field_0x84;
    u8 pad_0x85[3];
    void* field_0x88;
    void* field_0x90;
    s32 field_0x98;
    u8 field_0x9c;
    u8 pad_0x9d[3];
    s32 field_0xa0;
    u8 pad_0xa4[4];
    UnkAllocator* alloc_0xa8;
};

void fn_710008b88c(UnkState_0xb0* self) {
    self->alloc_0x0 = fn_7100083990();
    self->field_0x8 = 3;
    self->field_0xc = 0;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1c = 0;
    self->field_0x20 = 10;
    self->field_0x24 = 1;
    self->field_0x28 = 3;
    self->alloc_0x30 = fn_7100083990();
    self->field_0x38 = nullptr;
    self->field_0x40 = nullptr;
    self->vtable_0x48 = g_unk_7101a1a4c8 + 2;
    self->field_0x50 = nullptr;
    fn_710008377c(self->sub_0x58);
    self->field_0x80 = 0;
    self->field_0x84 = 0;
    self->field_0x88 = nullptr;
    self->field_0x90 = nullptr;
    self->field_0x98 = 10;
    self->field_0x9c = 1;
    self->field_0xa0 = 3;
    self->alloc_0xa8 = fn_7100083990();
}
